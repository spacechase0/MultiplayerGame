#include "client/LobbyClientController.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "client/Client.hpp"
#include "client/MatchClientController.hpp"
#include "Constants.hpp"
#include "net/Lobby/UnitAllocationPacket.hpp"

namespace client
{
    LobbyClientController::LobbyClientController( Client& theClient )
    :   ClientController::ClientController( theClient )
    {
        window.create( sf::VideoMode( 640, 480 ), "Game", sf::Style::Titlebar | sf::Style::Close );
        font.loadFromFile( "C:\\Windows\\Fonts\\arial.ttf" );

        allocated[ game::UnitType::Scout ] = 0;
        allocated[ game::UnitType::Fighter ] = 0;
        allocated[ game::UnitType::Mage ] = 0;
        allocated[ game::UnitType::Archer ] = 0;
    }

    void LobbyClientController::update()
    {
        ClientController::update();
        if ( !allocating )
        {
            window.close();
            return;
        }

        bool justClicked = false;
        bool rightClicked = false;
        sf::Vector2i clickPos( -1, -1 );
        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            if ( event.type == sf::Event::MouseButtonReleased )
            {
                justClicked = true;
                clickPos = sf::Vector2i( event.mouseButton.x, event.mouseButton.y );
                rightClicked = event.mouseButton.button == sf::Mouse::Right;
            }
            else if ( event.type == sf::Event::Closed )
                ; // TODO
        }

        sf::Text text( "", font, 20 );
        text.setFillColor( sf::Color::Black );

        window.clear( sf::Color::White );

        text.setString( util::format( "Points remaining: $", points ) );
        text.setPosition( 15, 15 );
        window.draw( text );

        text.setString( util::format( "Scout: $", allocated[ game::UnitType::Scout ] ) );
        text.setPosition( 15, 50 );
        window.draw( text );
        if ( justClicked && text.getGlobalBounds().contains( clickPos.x, clickPos.y ) )
        {
            if ( rightClicked && allocated[ game::UnitType::Scout ] > 0 )
            {
                --allocated[ game::UnitType::Scout ];
                ++points;
            }
            else if ( !rightClicked && points > 0 )
            {
                ++allocated[ game::UnitType::Scout ];
                --points;
            }
        }

        text.setString( util::format( "Fighter: $", allocated[ game::UnitType::Fighter ] ) );
        text.move( 0, 25 );
        window.draw( text );
        if ( justClicked && text.getGlobalBounds().contains( clickPos.x, clickPos.y ) )
        {
            if ( rightClicked && allocated[ game::UnitType::Fighter ] > 0 )
            {
                --allocated[ game::UnitType::Fighter ];
                ++points;
            }
            else if ( !rightClicked && points > 0 )
            {
                ++allocated[ game::UnitType::Fighter ];
                --points;
            }
        }

        text.setString( util::format( "Mage: $", allocated[ game::UnitType::Mage ] ) );
        text.move( 0, 25 );
        window.draw( text );
        if ( justClicked && text.getGlobalBounds().contains( clickPos.x, clickPos.y ) )
        {
            if ( rightClicked && allocated[ game::UnitType::Mage ] > 0 )
            {
                --allocated[ game::UnitType::Mage ];
                ++points;
            }
            else if ( !rightClicked && points > 0 )
            {
                ++allocated[ game::UnitType::Mage ];
                --points;
            }
        }

        text.setString( util::format( "Archer: $", allocated[ game::UnitType::Archer ] ) );
        text.move( 0, 25 );
        window.draw( text );
        if ( justClicked && text.getGlobalBounds().contains( clickPos.x, clickPos.y ) )
        {
            if ( rightClicked && allocated[ game::UnitType::Archer ] > 0 )
            {
                --allocated[ game::UnitType::Archer ];
                ++points;
            }
            else if ( !rightClicked && points > 0 )
            {
                ++allocated[ game::UnitType::Archer ];
                --points;
            }
        }

        text.setString( "Done" );
        text.setPosition( window.getSize().x - text.getLocalBounds().width - 15, window.getSize().y - text.getLocalBounds().height - 15 );
        window.draw( text );
        if ( justClicked && text.getGlobalBounds().contains( clickPos.x, clickPos.y ) )
        {
            client.log( "Done allocating units\n" );
            allocating = false;
            client.send( net::Lobby::UnitAllocationPacket( allocated ).toPacket() );
        }

        window.display();
    }

    void LobbyClientController::onPacket( sf::Packet& packet )
    {
        auto packetObj = net::Lobby::Packet::fromPacket( packet );
        if ( packetObj->id == net::Lobby::Id::Start )
        {
            client.log( "Match started\n" );
            controllerTransition( std::unique_ptr< ClientController >( new MatchClientController( client ) ) );
        }
    }
}
