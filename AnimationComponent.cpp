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

void LLGP::AnimationComponent::SetAnimationState(AnimationState state, std::unordered_map<std::string, sf::IntRect>& intRectMap, int numberOfFrames)
{
	m_currentState = state;
	m_currentMap = intRectMap;
	m_numberOfFrames = numberOfFrames;
	SetCurrentFrame(0);
}

void LLGP::AnimationComponent::SetNextSprite()
{
	m_sprite.setTextureRect(GetSpriteRectByName(m_name + std::to_string(m_currentFrame)));

	// Increment frame (after setting it)
	m_currentFrame = (m_currentFrame + 1) % m_numberOfFrames;
}

void LLGP::AnimationComponent::SetCurrentFrame(int currentFrame)
{
	m_currentFrame = currentFrame;
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
