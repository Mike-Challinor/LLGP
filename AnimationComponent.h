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
		int m_currentFrame = 0;
		int m_startingFrame = 0;
		int m_numberOfFrames;
		sf::Sprite& m_sprite;
		sf::Texture m_texture;
		std::unordered_map<std::string, sf::IntRect> m_currentMap;
		AnimationState m_currentState = idle;
		std::string m_name;

		sf::IntRect GetSpriteRectByName(const std::string& name) const;
		void SetNextSprite();
		void SetStartingFrame(int startingFrame);
	
	public:
		AnimationComponent(sf::Sprite& sprite, std::string name);
		~AnimationComponent();

		void SetAnimationState(AnimationState state, std::unordered_map<std::string, sf::IntRect>& intRectMap, int numberOfFrames, int startingFrame);
		AnimationState GetState();

		void Update();
	};
}

