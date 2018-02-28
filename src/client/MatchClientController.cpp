#include "client/MatchClientController.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "client/Client.hpp"
#include "Constants.hpp"
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
        }

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
                circle.setFillColor( sf::Color::White );
                circle.setOutlineColor( sf::Color::Black );
                if ( army.first != client.id )
                    circle.setOutlineColor( sf::Color::Red );
                circle.setOutlineThickness( 2 );
                circle.setOrigin( sf::Vector2f( 8, 8 ) );
                circle.setPosition( unit->pos.x, unit->pos.y );
                window.draw( circle );
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
    }
}
