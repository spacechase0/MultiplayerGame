#include "client/MatchClientController.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <util/Math.hpp>

#include "client/Client.hpp"
#include "Constants.hpp"
#include "net/Match/CommandPacket.hpp"
#include "net/Match/CurrentTurnPacket.hpp"
#include "net/Match/EndTurnPacket.hpp"
#include "net/Match/GameDataPacket.hpp"
#include "net/Match/Packet.hpp"

namespace
{
    constexpr float UNIT_OUTLINE = 2;
    constexpr float HEALTHBAR_WIDTH = game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE + UNIT_OUTLINE * 2;
    constexpr float HEALTHBAR_HEIGHT = 4;
}

namespace client
{
    MatchClientController::MatchClientController( Client& theClient )
    :   ClientController::ClientController( theClient )
    {
        window.create( sf::VideoMode( 640, 480 ), util::format( "Game - $", client.username ) );
        font.loadFromFile( "C:\\Windows\\Fonts\\arial.ttf" );
        view = window.getDefaultView();
    }

    void MatchClientController::update()
    {
        ClientController::update();

        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            if ( event.type == sf::Event::Resized )
            {
                view = sf::View( view.getCenter(), sf::Vector2f( event.size.width, event.size.height ) );
            }
            else if ( event.type == sf::Event::KeyPressed )
            {
                if ( event.key.code == sf::Keyboard::Z )
                    mouseMode = Select;
                else if ( event.key.code == sf::Keyboard::X )
                    mouseMode = Move;
                else if ( event.key.code == sf::Keyboard::C )
                    mouseMode = Attack;
                else if ( event.key.code == sf::Keyboard::Space )
                    client.send( net::Match::EndTurnPacket().toPacket() );
            }
            else if ( event.type == sf::Event::MouseButtonPressed )
            {
                if ( currentTurn == client.id )
                {
                    auto& army = armies[ client.id ];
                    if ( event.mouseButton.button == sf::Mouse::Left )
                    {
                        if ( mouseMode == Select )
                        {
                            bool newSel = false;
                            for ( auto& unit : army )
                            {
                                if ( unit.get() == selected )
                                    continue;

                                if ( util::distance( unit->pos * game::WORLD_UNIT_SIZE, sf::Vector2d( window.mapPixelToCoords( sf::Vector2i( event.mouseButton.x, event.mouseButton.y ), view ) ) ) <= 10 )
                                {
                                    newSel = true;
                                    selected = unit.get();
                                }
                            }

                            if ( !newSel )
                                selected = nullptr;
                        }
                        else if ( mouseMode == Move || mouseMode == Attack )
                        {
                            sf::Vector2d mousePos( window.mapPixelToCoords( sf::Vector2i( event.mouseButton.x, event.mouseButton.y ), view ) );
                            mousePos.x /= game::WORLD_UNIT_SIZE;
                            mousePos.y /= game::WORLD_UNIT_SIZE;
                            if ( mouseMode == Move )
                                selected->moveTo( this, mousePos );
                            else
                                selected->attack( this, mousePos );

                            auto it = army.begin();
                            for ( ; it != army.end(); ++it )
                                if ( it->get() == selected )
                                    break;
                            int unitId = it - army.begin();

                            net::Match::CommandPacket cmd;
                            cmd.type = mouseMode == Move ? net::Match::CommandPacket::Move : net::Match::CommandPacket::Attack;
                            cmd.withUnit = unitId;
                            cmd.pos = mousePos;
                            client.send( cmd.toPacket() );

                            if ( mouseMode == Attack || !sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ) && !sf::Keyboard::isKeyPressed( sf::Keyboard::RShift ) )
                            {
                                mouseMode = Select;
                                selected = nullptr;
                            }
                        }
                    }
                }
            }
        }

        if ( selected == nullptr || ( mouseMode == Move && selected->getMovementSpeedLeft() == 0 ) || ( mouseMode == Attack && selected->hasAttacked() ) )
            mouseMode = Select;

        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
            view.move( 0, -4 );
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
            view.move( -4, 0 );
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
            view.move( 0,  4 );
        if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
            view.move(  4, 0 );

        window.clear( sf::Color::White );
        window.setView( view );
        {
            sf::CircleShape circle;
            circle.setRadius( game::WORLD_UNIT_SIZE * game::WORLD_SPREAD * 1.1 );
            circle.setOrigin( circle.getRadius(), circle.getRadius() );
            circle.setOutlineColor( sf::Color( 0, 0, 100 ) );
            circle.setOutlineThickness( 3 );
            window.draw( circle );
        }
        for ( const auto& army : armies )
            for ( const auto& unit : army.second )
            {
                if ( unit->health <= 0 )
                    continue;

                sf::CircleShape circle;
                circle.setRadius( game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE / 2 );
                switch ( unit->type )
                {
                    case game::UnitType::Fighter: circle.setFillColor( sf::Color( 200, 0, 0 ) ); break;
                    case game::UnitType::Mage: circle.setFillColor( sf::Color::Blue ); break;
                    case game::UnitType::Archer: circle.setFillColor( sf::Color::Green ); break;
                    default: circle.setFillColor( sf::Color::White );
                }
                circle.setOutlineColor( sf::Color::Black );
                if ( army.first != client.id )
                    circle.setOutlineColor( sf::Color::Red );
                circle.setOutlineThickness( UNIT_OUTLINE );
                circle.setOrigin( sf::Vector2f( game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE / 2,
                                                game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE / 2 ) );
                circle.setPosition( unit->pos.x * game::WORLD_UNIT_SIZE, unit->pos.y * game::WORLD_UNIT_SIZE );
                window.draw( circle );

                if ( unit->health != unit->getMaxHealth() )
                {
                    sf::RectangleShape rect;
                    rect.setFillColor( sf::Color::Red );
                    rect.setOutlineColor( sf::Color::Red );
                    rect.setOutlineThickness( 1 );
                    rect.setSize( sf::Vector2f( HEALTHBAR_WIDTH, HEALTHBAR_HEIGHT ) );
                    rect.setPosition( circle.getPosition().x - game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE / 2 - UNIT_OUTLINE,
                                      circle.getPosition().y - game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE - UNIT_OUTLINE - HEALTHBAR_HEIGHT - 2 );
                    window.draw( rect );

                    float perc = unit->health / static_cast< float >( unit->getMaxHealth() );
                    rect.setSize( sf::Vector2f( HEALTHBAR_WIDTH * perc, HEALTHBAR_HEIGHT ) );
                    rect.setFillColor( sf::Color::Green );
                    rect.setOutlineThickness( 0 );
                    window.draw( rect );
                }

                if ( unit.get() == selected )
                {
                    circle.setRadius( game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE * 1.5f );
                    circle.setOrigin( sf::Vector2f( game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE * 1.5f,
                                                    game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE * 1.5f ) );
                    circle.setFillColor( sf::Color::Transparent );
                    circle.setOutlineColor( sf::Color::Yellow );
                    window.draw( circle );

                    if ( mouseMode == Move )
                    {
                        circle.setRadius( selected->getMovementSpeedLeft() * game::WORLD_UNIT_SIZE );
                        circle.setOrigin( selected->getMovementSpeedLeft() * game::WORLD_UNIT_SIZE, selected->getMovementSpeedLeft() * game::WORLD_UNIT_SIZE );
                        circle.setFillColor( sf::Color( 0, 255, 0, 64 ) );
                        circle.setOutlineThickness( 3 );
                        circle.setOutlineColor( sf::Color( 0, 255, 0, 192 ) );
                        window.draw( circle );
                    }
                    else if ( mouseMode == Attack )
                    {
                        circle.setRadius( selected->getAttackRange() * game::WORLD_UNIT_SIZE );
                        circle.setOrigin( selected->getAttackRange() * game::WORLD_UNIT_SIZE, selected->getAttackRange() * game::WORLD_UNIT_SIZE );
                        circle.setFillColor( sf::Color( 255, 00, 0, 64 ) );
                        circle.setOutlineThickness( 3 );
                        circle.setOutlineColor( sf::Color( 255, 0, 0, 192 ) );
                        window.draw( circle );
                    }
                }

                if ( unit->type == game::UnitType::Mage && unit->getChargeTime() > 0 && unit->getChargeTime() <= 1 )
                {
                    circle.setRadius( 4 * game::WORLD_UNIT_SIZE );
                    circle.setOrigin( sf::Vector2f( circle.getRadius(), circle.getRadius() ) );
                    circle.setFillColor( sf::Color::Transparent );
                    circle.setOutlineColor( sf::Color::Blue );
                    circle.setPosition( sf::Vector2f( unit->getChargeTarget() * game::WORLD_UNIT_SIZE ) );
                    window.draw( circle );
                }
            }

        window.setView( sf::View( sf::FloatRect( 0, 0, window.getSize().x, window.getSize().y ) ) );
        sf::Text text;
        text.setFont( font );
        text.setCharacterSize( 20 );
        text.setString( util::format( "Current turn: $", users[ currentTurn ] ) );
        text.setPosition( 15, 15 );
        text.setFillColor( sf::Color::Black );
        window.draw( text );

        int iy = 0;
        for ( const auto& unit : armies[ client.id ] )
        {
            if ( unit->health <= 0 )
                continue;

            sf::RectangleShape rect;
            rect.setFillColor( sf::Color( 200, 200, 200 ) );
            rect.setOutlineColor( unit.get() == selected ? sf::Color::Yellow : sf::Color::Black );
            rect.setOutlineThickness( 2 );
            rect.setSize( sf::Vector2f( 125, 50 ) );
            rect.setPosition( 20, 50 + 70 * iy );
            window.draw( rect );

            text.setCharacterSize( 15 );
            switch ( unit->type )
            {
                case game::UnitType::Scout:   text.setString( "Scout" );   break;
                case game::UnitType::Fighter: text.setString( "Fighter" ); break;
                case game::UnitType::Mage:    text.setString( "Mage" );    break;
                case game::UnitType::Archer:  text.setString( "Archer" );  break;
            }
            text.setPosition( rect.getPosition().x + 5 , rect.getPosition().y + 5 );
            window.draw( text );

            if ( !unit->hasAttacked() || unit->getChargeTime() > 0 )
            {
                sf::CircleShape circle;
                circle.setRadius( 3 );
                circle.setOrigin( sf::Vector2f( circle.getRadius(), circle.getRadius() ) );
                circle.setPosition( rect.getPosition().x + rect.getSize().x - 5 - circle.getRadius(), rect.getPosition().y + 5 + circle.getRadius() );
                circle.setFillColor( unit->getChargeTime() > 0 ? sf::Color::Blue : sf::Color::Red );
                circle.setOutlineThickness( 1 );
                circle.setOutlineColor( sf::Color::Black );
                window.draw( circle );
            }

            sf::CircleShape circle;
            circle.setRadius( game::ARMY_UNIT_SIZE * game::WORLD_UNIT_SIZE / 2 );
            switch ( unit->type )
            {
                case game::UnitType::Fighter: circle.setFillColor( sf::Color( 200, 0, 0 ) ); break;
                case game::UnitType::Mage: circle.setFillColor( sf::Color::Blue ); break;
                case game::UnitType::Archer: circle.setFillColor( sf::Color::Green ); break;
                default: circle.setFillColor( sf::Color::White );
            }
            circle.setOutlineColor( sf::Color::Black );
            circle.setOutlineThickness( UNIT_OUTLINE );
            circle.setOrigin( sf::Vector2f( circle.getRadius(), circle.getRadius() ) );
            circle.setPosition( rect.getPosition().x + 10 + circle.getRadius(), rect.getPosition().y + 5 + 15 + 5 + circle.getRadius() );
            window.draw( circle );

            int rsx = rect.getSize().x;

            rect.setSize( sf::Vector2f( rsx - circle.getRadius() * 2 - 30, 8 ) );
            rect.setPosition( rect.getPosition().x + circle.getRadius() * 2 + 25, rect.getPosition().y + 5 + 15 + 5 );
            rect.setFillColor( sf::Color( 100, 0, 0 ) );
            rect.setOutlineThickness( 0 );
            rect.setOutlineColor( sf::Color::Transparent );
            window.draw( rect );
            rect.setSize( sf::Vector2f( rect.getSize().x * ( static_cast< float >( unit->health ) / unit->getMaxHealth() ), rect.getSize().y ) );
            rect.setFillColor( sf::Color( 0, 200, 0 ) );
            window.draw( rect );

            rect.move( 0, 12 );
            rect.setSize( sf::Vector2f( rsx - circle.getRadius() * 2 - 30, 8 ) );
            rect.setFillColor( sf::Color( 100, 100, 0 ) );
            window.draw( rect );
            if ( unit->getMovementSpeedLeft() > 0 )
            {
                rect.setSize( sf::Vector2f( rect.getSize().x * ( unit->getMovementSpeedLeft() / unit->getMovementSpeedPerTurn() ), rect.getSize().y ) );
                rect.setFillColor( sf::Color( 200, 200, 0 ) );
                window.draw( rect );
            }

            ++iy;
        }

        window.display();
    }

    std::vector< game::Unit* > MatchClientController::getUnitsAt( sf::Vector2d pos )
    {
        return getUnitsWithin( pos, game::ARMY_UNIT_SIZE );
    }

    std::vector< game::Unit* > MatchClientController::getUnitsWithin( sf::Vector2d pos, double dist )
    {
        std::vector< game::Unit* > ret;
        for ( auto& army : armies )
            for ( auto& unit : army.second )
                if ( util::distance( unit->pos, pos ) < dist )
                    ret.push_back( unit.get() );
        return ret;
    }

    std::vector< game::Unit* > MatchClientController::getUnitsIntersecting( sf::Vector2d start, sf::Vector2d end )
    {
        sf::Vector2d left = start, right = end;
        sf::Vector2d top = start, bottom = end;
        if ( right.x < left.x ) std::swap( left, right );
        if ( bottom.y < top.y ) std::swap( top, bottom );

        std::vector< game::Unit* > ret;
        for ( auto& army : armies )
            for ( auto& unit : army.second )
            {
                // https://stackoverflow.com/a/23017208/1687492
                double dx = end.x - start.x;
                double dy = end.y - start.y;

                double a = dx * dx + dy * dy;
                double b = 2 * ( dx * ( start.x - unit->pos.x ) + dy * ( start.y - unit->pos.y ) );
                double c = ( start.x - unit->pos.x ) * ( start.x - unit->pos.x ) + ( start.y - unit->pos.y ) * ( start.y - unit->pos.y ) - game::ARMY_UNIT_SIZE * game::ARMY_UNIT_SIZE;

                double det = b * b - 4 * a * c;
                if ( a <= 0.0000001 || det < 0 )
                {
                    continue;
                }
                else
                {
                    if ( unit->pos.x < left.x || unit->pos.x > right.x ||
                         unit->pos.y < top.y || unit->pos.y > bottom.y )
                        continue;
                    ret.push_back( unit.get() );
                }
            }
        return ret;
    }

    void MatchClientController::onPacket( sf::Packet& packet )
    {
        auto packetObj = net::Match::Packet::fromPacket( packet );
        if ( packetObj->id == net::Match::Id::GameData )
        {
            client.log( "Received match state!\n" );

            auto data = static_cast< net::Match::GameDataPacket* >( packetObj.get() );
            if ( data->users.size() > 0 )
                users.clear();
            armies.clear();

            users = data->users;

            for ( const auto& army : data->armies )
                for ( const auto& unitData : army.second )
                {
                    auto unit = std::make_unique< game::Unit >( unitData.type );
                    unit->pos = unitData.pos;
                    unit->health = unitData.health;

                    armies[ army.first ].push_back( std::move( unit ) );
                }
        }
        else if ( packetObj->id == net::Match::Id::CurrentTurn )
        {
            auto turn = static_cast< net::Match::CurrentTurnPacket* >( packetObj.get() );

            client.log( "Current turn: $\n", users[ turn->current ] );
            currentTurn = turn->current;

            for ( auto& unit : armies[ currentTurn ] )
                unit->reset( this );
        }
        else if ( packetObj->id == net::Match::Id::Command )
        {
            auto cmd = static_cast< net::Match::CommandPacket* >( packetObj.get() );
            switch ( cmd->type )
            {
                case net::Match::CommandPacket::Move:
                    client.log( "Moving unit $ towards ($, $)\n", static_cast< int >( cmd->withUnit ), cmd->pos.x, cmd->pos.y );
                    armies[ currentTurn ][ cmd->withUnit ]->moveTo( this, cmd->pos );
                    break;
                case net::Match::CommandPacket::Attack:
                    client.log( "Attacking with unit $ at ($, $)\n", static_cast< int >( cmd->withUnit ), cmd->pos.x, cmd->pos.y );
                    armies[ currentTurn ][ cmd->withUnit ]->attack( this, cmd->pos );
                    break;
            }
        }
    }
}
