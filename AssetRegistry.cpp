#include "AssetRegistry.h"

void LLGP::AssetRegistry::PopulateTextureMap(std::unordered_map<std::string, sf::IntRect>& texture_map, 
	int amount, 
	const std::string& name, 
	int texture_width, 
	int texture_height, 
	int starting_x_pos, 
	int starting_y_pos,
	int padding)
{
	int currentXPos = starting_x_pos;
	int currentYPos = starting_y_pos;

	InitFonts();

	// Loop through for each individual sprite
	for (int i = 0; i < amount; i++)
	{
		// Set the temp int rect
		sf::IntRect tempIntRect({ currentXPos, currentYPos }, { texture_width , texture_height });

		// Add to the unordered map
		texture_map.insert({ name + std::to_string(i), tempIntRect });

		// Increment the current x position
		currentXPos += texture_width;
		currentXPos += padding;
	}
}

void LLGP::AssetRegistry::InitFonts()
{
	/*
	if (!m_joustFont.openFromFile("Resources/Fonts/JoustFont.otf"))
	{
		throw std::runtime_error("ERROR::ASSETREGISTRY::INITFONTS:: Failed to load JoustFontotf");
	}*/
	
}

void LLGP::AssetRegistry::StoreTextureMaps()
{
	// Load the ostrich textures
	PopulateTextureMap(m_ostrichTextures, 7, "ostrich", 30, 40, 2, 12, 2);

	// Load the stork textures
	PopulateTextureMap(m_storkTextures, 7, "stork", 28, 40, 2, 76, 4);
}

void LLGP::AssetRegistry::LoadSpriteSheet()
{
	// Set player texture
	if (!this->m_spriteSheet.loadFromFile("Resources/Sprites/JoustSpriteSheet.png"))
	{
		throw std::runtime_error("Failed to load texture: joustsprites.png");
	}

	else
	{
		StoreTextureMaps();
	}
}

sf::Font LLGP::AssetRegistry::GetFont()
{
	return m_joustFont;	
}

sf::Texture& LLGP::AssetRegistry::LoadTexture()
{
	return m_spriteSheet;
}

const std::unordered_map<std::string, sf::IntRect>& LLGP::AssetRegistry::LoadPlayerSprites(int player_id)
{
	switch (player_id)
	{
	case 1: 
		return m_ostrichTextures;
		break;

	case 2:
		return m_storkTextures;
		break;

	default:
		throw std::runtime_error("Player ID is invalid, unable to load player sprites");
	}
	
}
