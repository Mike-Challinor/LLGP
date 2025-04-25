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

void LLGP::AssetRegistry::AddSpriteToMap(std::unordered_map<std::string, sf::IntRect>& texture_map, 
	const std::string& name, int xPos, int yPos, int width, int height)
{
	// Set the temp int rect
	sf::IntRect tempIntRect({ xPos, yPos }, { width , height });

	// Add to the unordered map
	texture_map.insert({ name + std::to_string(texture_map.size()), tempIntRect });
}

void LLGP::AssetRegistry::StoreTextureMaps()
{
	// Load the ostrich textures
	PopulateTextureMap(m_ostrichTextures, 7, "ostrich", 30, 40, 2, 12, 2);

	// Load the stork textures
	PopulateTextureMap(m_storkTextures, 7, "stork", 28, 40, 2, 76, 4);

	// Load the platform textures
	AddSpriteToMap(m_bottom_platformTextures, "bottom_platform", 0, 704, 372, 60); // Bottom platform
	AddSpriteToMap(m_top_left_platformTextures, "top_left_platform", 0, 640, 65, 14); // Top left platform
	AddSpriteToMap(m_bottom_left_platformTextures, "bottom_left_platform", 384, 640, 126, 16); // Bottom left platform
	AddSpriteToMap(m_bottom_middle_platformTextures, "bottom_middle_platform", 736, 640, 128, 16); // Bottom left platform

	// Load the vulture textures
	PopulateTextureMap(m_enemyTextures, 4, "enemy", 30, 28, 2, 149, 1);
	AddSpriteToMap(m_enemyTextures, "enemy", 128, 150, 28, 26); // Sliding sprite
	AddSpriteToMap(m_enemyTextures, "enemy", 160, 152, 32, 16); // Flying sprite 1
	AddSpriteToMap(m_enemyTextures, "enemy", 192, 144, 33, 26); // Flying sprite 2

}

void LLGP::AssetRegistry::LoadSpriteSheet()
{
	// Set player texture
	if (!this->m_spriteSheet.loadFromFile("Resources/Sprites/JoustSpriteSheet2.png"))
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

const std::unordered_map<std::string, sf::IntRect>& LLGP::AssetRegistry::LoadPlatformSprites()
{
	return m_top_left_platformTextures;
}

const std::unordered_map<std::string, sf::IntRect>& LLGP::AssetRegistry::LoadSprites(sf::String name)
{
	if (name == "ostrich")
	{
		return m_ostrichTextures;
	}
	else if (name == "stork")
	{
		return m_storkTextures;
	}
	else if (name == "bottom_platform")
	{
		return m_bottom_platformTextures;
	}
	else if (name == "top_left_platform")
	{
		return m_top_left_platformTextures;
	}
	else if (name == "bottom_left_platform")
	{
		return m_bottom_left_platformTextures;
	}
	else if (name == "bottom_middle_platform")
	{
		return m_bottom_middle_platformTextures;
	}
	else if (name == "enemy" || name == "bounder" || name == "hunter" || name == "shadowlord")
	{
		return m_enemyTextures;
	}
	else
	{
		throw std::runtime_error("Invalid sprite name provided to LoadSprites: " + name);
	}

}
