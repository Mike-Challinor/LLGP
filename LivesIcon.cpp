#include "LivesIcon.h"

// --- Contructor ---
LivesIcon::LivesIcon(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName, int spriteIndex) 
	: GameObject(assetRegistry, xPos, yPos, objectName)
{
	m_spriteIndex = spriteIndex;
	SetTexture();
}

// --- Destructor ---
LivesIcon::~LivesIcon()
{
}

// --- Set the texture of the base sprite (overidden from GameObject class) ---
void LivesIcon::SetTexture()
{
	m_texture = m_assetRegistry.LoadTexture();
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(m_assetRegistry.LoadSprite("rider", m_spriteIndex));
}
