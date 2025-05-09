#pragma once



#include <SFML/Graphics.hpp>

namespace LLGP
{
	enum AnimationState
	{
		idle,
		walking,
		skidding,
		flying
	};

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
		AnimationComponent(sf::Sprite& sprite, std::string name);
		~AnimationComponent();

		void SetAnimationState(AnimationState state, std::unordered_map<std::string, sf::IntRect>& intRectMap, 
			int numberOfFrames, int startingFrame, float frameTime = 0.1f);
		AnimationState GetState();

		int GetCurrentFrame();

		void Update(float deltaTime);
	};
}

