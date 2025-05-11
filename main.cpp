#include <SFML/Graphics.hpp>
#include <chrono>
#include "Constants.h"
#include "player.h"
#include "Game.h"
#include "AssetRegistry.h"

using namespace std;

int WinMain()
{
    // Create window
    sf::VideoMode videoMode = sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT });
    sf::RenderWindow window(videoMode, "SFML works!");

    // Create references to classes
    LLGP::InputManager inputManager;
    LLGP::AssetRegistry assetRegistry;
    unique_ptr<Game> gameInstance = make_unique<Game>(inputManager, assetRegistry);

    // Set variables for fixed update
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    float deltaTime = 0.0f;
    float timeSinceLastPhysicsLoop = 0.0f;
    float physicsTimeStep = 0.016f;
    float inputTimeStep = 0.016f;
    float timeSinceLastInputLoop = 0.0f;

    // Main loop
    while (window.isOpen())
    {
        // Update time related floats
        auto now = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration<float>(now - lastTime).count(); // Convert to seconds
        lastTime = now;

        timeSinceLastPhysicsLoop += deltaTime;
        timeSinceLastInputLoop += deltaTime;

        // Physics loop (fixed update)
        while (timeSinceLastPhysicsLoop > physicsTimeStep)
        {
            // Update the game
            gameInstance->Update(deltaTime);

            timeSinceLastPhysicsLoop -= physicsTimeStep;
        }

        // Input loop (fixed update)
        while (timeSinceLastInputLoop > inputTimeStep)
        {
            // Update games input
            gameInstance->UpdateInputs();

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
        
        // Render the game
        gameInstance->Render(window);

        // Update the window
        window.display();
    }
}