#include "AnimationComponent.h"


LLGP::AnimationComponent::AnimationComponent(sf::Sprite& sprite, std::string name)
	: m_sprite(sprite)
{
	m_name = name;
	m_numberOfFrames = 0;
}


LLGP::AnimationComponent::~AnimationComponent()
{
}

void LLGP::AnimationComponent::SetAnimationState(AnimationState state, std::unordered_map<std::string, sf::IntRect>& intRectMap, int numberOfFrames, int startingFrame)
{
	m_currentState = state;
	m_currentMap = intRectMap;
	m_numberOfFrames = numberOfFrames;
	SetStartingFrame(startingFrame);

}

LLGP::AnimationState LLGP::AnimationComponent::GetState()
{
	return m_currentState;
}

void LLGP::AnimationComponent::SetNextSprite()
{
	m_sprite.setTextureRect(GetSpriteRectByName(m_name + std::to_string(m_currentFrame)));

	// Increment frame (after setting it)
	//m_currentFrame = (m_currentFrame + 1) % m_numberOfFrames;
	int relativeFrame = (m_currentFrame - m_startingFrame + 1) % m_numberOfFrames;
	m_currentFrame = m_startingFrame + relativeFrame;
}

void LLGP::AnimationComponent::SetStartingFrame(int startingFrame)
{
	m_startingFrame = startingFrame;
	m_currentFrame = startingFrame;
}

void LLGP::AnimationComponent::Update()
{
	SetNextSprite();
}

sf::IntRect LLGP::AnimationComponent::GetSpriteRectByName(const std::string& name) const
{
	auto it = m_currentMap.find(name);
	if (it != m_currentMap.end())
	{
		return it->second; // Found the sprite
	}

	// Handle missing sprite
	throw std::runtime_error("Sprite not found: " + name);
}
