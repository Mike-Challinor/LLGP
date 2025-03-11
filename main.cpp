#include <SFML/Graphics.hpp>
#include "player.h"
#include <chrono>

using namespace std;

/*
    This version of the SFML "hello world" is statically linked, you may wish to try the dynamically linked version as well.
*/

int WinMain()
{
    sf::VideoMode videoMode = sf::VideoMode({ 800, 600 });
    sf::RenderWindow window(videoMode, "SFML works!");

    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    float deltaTime = 0;
    float timeSinceLastPhysicsLoop = 0;
    float physicsTimeStep = 2000;

    unique_ptr<player> playerClass = make_unique<player>(10.f, 10.f);

    while (window.isOpen())
    {
        // Fixed update loop: Time Step stuff
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - lastTime).count();
        lastTime = std::chrono::steady_clock::now();
        timeSinceLastPhysicsLoop += deltaTime;

        // Physics loop
        while (timeSinceLastPhysicsLoop > physicsTimeStep)
        {
            // Update the player
            playerClass->update(videoMode, deltaTime);
            timeSinceLastPhysicsLoop -= physicsTimeStep;
        }

        // Event polling section of code - this must be done in the thread which created the window
        // we will talk about threading later, but essentially this must be done here
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

        }

        // Clear the screen
        window.clear();
        
        // Render the player
        playerClass->render(window);

        // Update the window
        window.display();
    }
}