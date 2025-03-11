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

    float timeStep = 10;
    float moveSpeed = 50; // 50 pixels per second
    sf::Time lastTime = sf::Time();
    sf::Time currentTime = sf::Time();
    sf::Clock timer = sf::Clock();
    timer.start();

    currentTime = timer.getElapsedTime();

    unique_ptr<player> playerClass = make_unique<player>(10.f, 10.f);

    while (window.isOpen())
    {
        // Event polling section of code - this must be done in the thread which created the window
        // we will talk about threading later, but essentially this must be done here
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

        }

        // Fixed update loop: Time Step stuff
        currentTime = timer.getElapsedTime();

        float deltaTime = currentTime.asMilliseconds() - lastTime.asMilliseconds();
        {
            lastTime = currentTime;

            // Update the player
            playerClass->update(videoMode, deltaTime);
        }

        // Clear the screen
        window.clear();
        
        // Render the player
        playerClass->render(window);

        // Update the window
        window.display();
    }
}