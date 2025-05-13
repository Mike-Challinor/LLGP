#pragma once



#include <SFML/Graphics.hpp>

namespace LLGP
{
	// Enums
	enum AnimationState
	{
		idle,
		walking,
		skidding,
		flying
	};

	// Convert animation state to string
	inline std::string ToString(AnimationState state)
	{
		switch (state)
		{
		case AnimationState::idle:    return "idle";
		case AnimationState::walking: return "walking";
		case AnimationState::skidding: return "skidding";
		case AnimationState::flying: return "flying";
		default:                      return "Unknown";
		}
	}

	class AnimationComponent
	{
	private:
		// Ints
		int m_currentFrame = 0;
		int m_startingFrame = 0;
		int m_numberOfFrames;

		// Floats
		float m_frameTime = 0.1f;
		float m_timeSinceLastFrame = 0.f;

		// Shapes
		sf::Sprite& m_sprite;
		sf::Texture m_texture;

		// Maps
		std::unordered_map<std::string, sf::IntRect> m_currentMap;

		// AnimationsState
		AnimationState m_currentState = idle;

		// Strings
		std::string m_name;

		// Accessor functions
		sf::IntRect GetSpriteRectByName(const std::string& name) const;

		// Modifier functions
		void SetNextSprite();
		void SetStartingFrame(int startingFrame);
		void SetFrameTime(float timePerFrame);
	
	public:
		// Constructor and Destructor
		AnimationComponent(sf::Sprite& sprite, std::string name);
		~AnimationComponent();

		// Functions
		void SetAnimationState(AnimationState state, std::unordered_map<std::string, sf::IntRect>& intRectMap, 
			int numberOfFrames, int startingFrame, float frameTime = 0.1f);

		// Accessor functions
		AnimationState GetState();
		int GetCurrentFrame();

		// Update functions
		void Update(float deltaTime);
	};
}

