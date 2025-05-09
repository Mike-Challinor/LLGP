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

void LLGP::AnimationComponent::SetAnimationState(AnimationState state, std::unordered_map<std::string, sf::IntRect>& intRectMap, 
	int numberOfFrames, int startingFrame, float frameTime)
{
	m_currentState = state;
	m_currentMap = intRectMap;
	m_numberOfFrames = numberOfFrames;
	SetStartingFrame(startingFrame);
	SetFrameTime(frameTime);

	m_timeSinceLastFrame = 0.f; // Reset timer
	m_sprite.setTextureRect(GetSpriteRectByName(m_name + std::to_string(m_currentFrame))); // Set animation to the first frame
}

LLGP::AnimationState LLGP::AnimationComponent::GetState()
{
	return m_currentState;
}

int LLGP::AnimationComponent::GetCurrentFrame()
{
	return m_currentFrame;
}

void LLGP::AnimationComponent::SetNextSprite()
{
	m_sprite.setTextureRect(GetSpriteRectByName(m_name + std::to_string(m_currentFrame)));

	int relativeFrame = (m_currentFrame - m_startingFrame + 1) % m_numberOfFrames;
	m_currentFrame = m_startingFrame + relativeFrame;
}

void LLGP::AnimationComponent::SetStartingFrame(int startingFrame)
{
	m_startingFrame = startingFrame;
	m_currentFrame = startingFrame;
}

void LLGP::AnimationComponent::SetFrameTime(float timePerFrame)
{
	m_frameTime = timePerFrame;
}

void LLGP::AnimationComponent::Update(float deltaTime)
{
	m_timeSinceLastFrame += deltaTime;

	if (m_timeSinceLastFrame >= m_frameTime)
	{
		SetNextSprite();
		m_timeSinceLastFrame = 0.f;
	}
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
