#include "AnimationComponent.h"

// --- Constructor ---
LLGP::AnimationComponent::AnimationComponent(sf::Sprite& sprite, std::string name)
	: m_sprite(sprite)
{
	m_name = name;
	m_numberOfFrames = 0;
}

// --- Destructor ---
LLGP::AnimationComponent::~AnimationComponent()
{
}

// --- Set to the new animation state ---
void LLGP::AnimationComponent::SetAnimationState(AnimationState state, std::unordered_map<std::string, sf::IntRect>& intRectMap, 
	int numberOfFrames, int startingFrame, float frameTime)
{
	m_currentState = state;
	m_currentMap = intRectMap;
	m_numberOfFrames = numberOfFrames;
	SetStartingFrame(startingFrame);
	SetFrameTime(frameTime); // Length of time per frame

	m_timeSinceLastFrame = 0.f; // Reset timer
	m_sprite.setTextureRect(GetSpriteRectByName(m_name + std::to_string(m_currentFrame))); // Set animation to the first frame
}

// --- Function to return the current state ---
LLGP::AnimationState LLGP::AnimationComponent::GetState()
{
	return m_currentState;
}

// --- Function to return the current frame number ---
int LLGP::AnimationComponent::GetCurrentFrame()
{
	return m_currentFrame;
}

// --- Function that sets the sprites texture rect to the next frame ---
void LLGP::AnimationComponent::SetNextSprite()
{
	m_sprite.setTextureRect(GetSpriteRectByName(m_name + std::to_string(m_currentFrame)));

	int relativeFrame = (m_currentFrame - m_startingFrame + 1) % m_numberOfFrames;
	m_currentFrame = m_startingFrame + relativeFrame;
}

// --- Function that sets to the starting frame of the animation ---
void LLGP::AnimationComponent::SetStartingFrame(int startingFrame)
{
	m_startingFrame = startingFrame;
	m_currentFrame = startingFrame;
}

// --- Function that sets the amount of time per frame ---
void LLGP::AnimationComponent::SetFrameTime(float timePerFrame)
{
	m_frameTime = timePerFrame;
}

// --- Function to update the component ---
void LLGP::AnimationComponent::Update(float deltaTime)
{
	m_timeSinceLastFrame += deltaTime; // Increase time since last frame by delta time

	if (m_timeSinceLastFrame >= m_frameTime)
	{
		SetNextSprite();
		m_timeSinceLastFrame = 0.f; // Reset the timer
	}
}

// --- Function that returns the int rect of the desired sprite by name ---
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
