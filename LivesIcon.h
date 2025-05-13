#pragma once
#include "GameObject.h"

class LivesIcon : public GameObject
{
public:
	// Constructor & Destructor
	LivesIcon(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName, int spriteIndex);
	virtual ~LivesIcon();

private:
	// Modifier functions
	void SetTexture() override;

	// Ints
	int m_spriteIndex;
};

