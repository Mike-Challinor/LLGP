#include <SFML/Graphics.hpp>
#include "player.h"
#include "InputManager.h"
#include <chrono>

using namespace std;

/*
    This version of the SFML "hello world" is statically linked, you may wish to try the dynamically linked version as well.
*/

int WinMain()
{
    sf::VideoMode videoMode = sf::VideoMode({ 800, 600 });
    sf::RenderWindow window(videoMode, "SFML works!");

    // Player ref
    unique_ptr<player> playerClass = make_unique<player>(10.f, 10.f);

    // Add listeners for keys
    inputManager.AddKeyListener(LLGP::Key::W, contextObj, [&]() { playerListener(LLGP::Key::W); });
    inputManager.AddKeyListener(LLGP::Key::A, contextObj, [&]() { playerListener(LLGP::Key::A); });

    // Simulate key press events
    inputManager.OnKeyPress(LLGP::Key::W);
    inputManager.OnKeyPress(LLGP::Key::A);

    // Remove listeners if necessary
    inputManager.RemoveKeyListener(LLGP::Key::W, contextObj, [&]() { playerListener(LLGP::Key::W); });

    // Simulate another key press after removing the listener
    inputManager.OnKeyPress(LLGP::Key::W);

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
            playerClass->update(videoMode, deltaTime);
            timeSinceLastPhysicsLoop -= physicsTimeStep;
        }

        // Input loop (fixed update)
        while (timeSinceLastInputLoop > inputTimeStep)
        {
            // Update players input
            playerClass->updateInput();
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
        playerClass->render(window);

        // Update the window
        window.display();
    }
}