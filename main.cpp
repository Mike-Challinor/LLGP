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

    LLGP::InputManager inputManager;

    // Player ref
    unique_ptr<player> playerClass1 = make_unique<player>(inputManager, 10.f, 10.f, 1);
    unique_ptr<player> playerClass2 = make_unique<player>(inputManager, 180.f, 10.f, 2);

    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    float deltaTime = 0;
    float timeSinceLastPhysicsLoop = 0;
    float physicsTimeStep = 2000;
    float inputTimeStep = 2000;
    float timeSinceLastInputLoop = 0;

    

    while (window.isOpen())
    {
        // Update time related floats
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - lastTime).count();
        lastTime = std::chrono::steady_clock::now();
        timeSinceLastPhysicsLoop += deltaTime;
        timeSinceLastInputLoop += deltaTime;

        // Physics loop (fixed update)
        while (timeSinceLastPhysicsLoop > physicsTimeStep)
        {
            // Update the player
            playerClass1->update(videoMode, deltaTime);
            playerClass2->update(videoMode, deltaTime);
            timeSinceLastPhysicsLoop -= physicsTimeStep;
        }

        // Input loop (fixed update)
        while (timeSinceLastInputLoop > inputTimeStep)
        {
            // Update players input
            playerClass1->updateInput();
            playerClass2->updateInput();
            timeSinceLastInputLoop -= inputTimeStep;
        }

        // Event polling section of code
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

        }

        // Clear the screen
        window.clear();
        
        // Render the player
        playerClass1->render(window);
        playerClass2->render(window);

        // Update the window
        window.display();
    }
}