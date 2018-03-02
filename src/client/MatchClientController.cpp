#include "client/MatchClientController.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <util/Math.hpp>

#include "client/Client.hpp"
#include "Constants.hpp"
#include "net/Match/CurrentTurnPacket.hpp"
#include "net/Match/GameDataPacket.hpp"
#include "net/Match/Packet.hpp"

namespace client
{
    MatchClientController::MatchClientController( Client& theClient )
    :   ClientController::ClientController( theClient )
    {
        window.create( sf::VideoMode( 640, 480 ), "Game" );
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
            }
            else if ( event.type == sf::Event::MouseButtonPressed )
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

                        if (  !newSel )
                            selected = nullptr;
                    }
                    else if ( mouseMode == Move )
                    {
                        sf::Vector2d mousePos( window.mapPixelToCoords( sf::Vector2i( event.mouseButton.x, event.mouseButton.y ), view ) );
                        mousePos.x /= game::WORLD_UNIT_SIZE;
                        mousePos.y /= game::WORLD_UNIT_SIZE;
                        selected->moveTo( mousePos );

                        if ( !sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ) && !sf::Keyboard::isKeyPressed( sf::Keyboard::RShift ) )
                        {
                            mouseMode = Select;
                            selected = nullptr;
                        }
                    }
                    else if ( mouseMode == Attack )
                    {
                        // TODO
                    }
                }
            }
        }

        if ( selected == nullptr || ( mouseMode == Move && selected->getMovementSpeedLeft() == 0 ) )
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
        for ( const auto& army : armies )
            for ( const auto& unit : army.second )
            {
                sf::CircleShape circle;
                circle.setRadius( 8 );
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
                circle.setOutlineThickness( 2 );
                circle.setOrigin( sf::Vector2f( 8, 8 ) );
                circle.setPosition( unit->pos.x * game::WORLD_UNIT_SIZE, unit->pos.y * game::WORLD_UNIT_SIZE );
                window.draw( circle );

                if ( unit.get() == selected )
                {
                    circle.setRadius( 12 );
                    circle.setOrigin( sf::Vector2f( 12, 12 ) );
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
            }
        window.display();
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
            auto data = static_cast< net::Match::CurrentTurnPacket* >( packetObj.get() );

            client.log( "Current turn: $\n", users[ data->current ] );
        }
    }
}
