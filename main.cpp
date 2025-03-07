#include <SFML/Graphics.hpp>
#include "player.h"

using namespace std;

/*
    This version of the SFML "hello world" is statically linked, you may wish to try the dynamically linked version as well.
*/

int WinMain()
{
    sf::VideoMode videoMode = sf::VideoMode({ 800, 600 });
    sf::RenderWindow window(videoMode, "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    float movementSpeed = 5.f;

    unique_ptr<player> playerClass = make_unique<player>(10.f, 10.f);

    while (window.isOpen())
    {
        playerClass->update(videoMode);

        // Event polling section of code - this must be done in the thread which created the window
        // we will talk about threading later, but essentially this must be done here
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>())
            {
                playerClass->updateInput();
            }
        }

        // Clear the screen
        window.clear();

        // Draw the object
        window.draw(shape);

        // Update the window
        window.display();
    }
}