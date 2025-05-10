#pragma once
#include "GameObject.h"

class LivesIcon : public GameObject
{
public:
	// Constructor & Destructor
	LivesIcon(LLGP::AssetRegistry& assetRegistry, float xPos, float yPos, const std::string& objectName, int spriteIndex);
	virtual ~LivesIcon();

private:
	void SetTexture() override;

	int m_spriteIndex;
};

