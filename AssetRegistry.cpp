#include "AssetRegistry.h"

// --- Constructor ---
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

// --- Function for loading the font from file ---
void LLGP::AssetRegistry::InitFonts()
{
	if (!m_joustFont.openFromFile("Resources/Fonts/joust.otf"))
	{
		throw std::runtime_error("ERROR::ASSETREGISTRY::INITFONTS:: Failed to load joust.otf");
	}
	
}

// --- Function for adding a single sprite to a map ---
void LLGP::AssetRegistry::AddSpriteToMap(std::unordered_map<std::string, sf::IntRect>& texture_map, 
	const std::string& name, int xPos, int yPos, int width, int height)
{
	// Set the temp int rect
	sf::IntRect tempIntRect({ xPos, yPos }, { width , height });

	// Add to the unordered map
	texture_map.insert({ name + std::to_string(texture_map.size()), tempIntRect });
}

// --- Function for setting all texture maps ---
void LLGP::AssetRegistry::StoreTextureMaps()
{
	// Load the ostrich textures
	PopulateTextureMap(m_ostrichTextures, 7, "ostrich", 30, 40, 2, 12, 2);

	// Load the stork textures
	PopulateTextureMap(m_storkTextures, 7, "stork", 28, 40, 2, 76, 4);

	// Load the rider textures
	PopulateTextureMap(m_riderTextures, 3, "rider", 24, 14, 6, 214, 8);

	// Load the vulture textures
	PopulateTextureMap(m_enemyTextures, 4, "enemy", 30, 28, 2, 149, 1);
	AddSpriteToMap(m_enemyTextures, "enemy", 128, 150, 28, 26); // Sliding sprite
	AddSpriteToMap(m_enemyTextures, "enemy", 160, 152, 32, 16); // Flying sprite 1
	AddSpriteToMap(m_enemyTextures, "enemy", 192, 144, 33, 26); // Flying sprite 2

	// Load the platform textures
	AddSpriteToMap(m_bottom_platformTextures, "bottom_platform", 0, 704, 372, 60); // Bottom platform
	AddSpriteToMap(m_top_left_platformTextures, "top_left_platform", 0, 640, 65, 14); // Top left platform
	AddSpriteToMap(m_bottom_left_platformTextures, "bottom_left_platform", 384, 640, 126, 16); // Bottom left platform
	AddSpriteToMap(m_bottom_middle_platformTextures, "bottom_middle_platform", 736, 640, 128, 16); // Bottom middle platform
	AddSpriteToMap(m_top_middle_platformTextures, "top_middle_platform", 192, 640, 176, 18); // Top middle platform
	AddSpriteToMap(m_middle_right_platformTextures, "middle_right_platform", 512, 640, 116, 22); // Middle right platform
	AddSpriteToMap(m_top_right_platformTextures, "top_right_platform", 96, 640, 94, 14); // Top right platform
	AddSpriteToMap(m_bottom_right_platformTextures, "bottom_right_platform", 640, 640, 92, 15); // Bottom right platform

}

// --- Function for loading the sprite sheet from file ---
void LLGP::AssetRegistry::LoadSpriteSheet()
{
	// Set player texture
	if (!this->m_spriteSheet.loadFromFile("Resources/Sprites/JoustSpriteSheet3.png"))
	{
		throw std::runtime_error("Failed to load texture: joustsprites.png"); // Handle errors from file not loading
	}

	else
	{
		StoreTextureMaps();
	}
}

// --- Function that returns the loaded font ---
sf::Font LLGP::AssetRegistry::GetFont()
{
	return m_joustFont;	
}

// --- Function for returning the loaded sprite sheet ---
sf::Texture& LLGP::AssetRegistry::LoadTexture()
{
	return m_spriteSheet;
}

// --- Function that returns the sprite map depending on the name passed through ---
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
	else if (name == "top_middle_platform")
	{
		return m_top_middle_platformTextures;
	}
	else if (name == "middle_right_platform")
	{
		return m_middle_right_platformTextures;
	}
	else if (name == "top_right_platform")
	{
		return m_top_right_platformTextures;
	}
	else if (name == "bottom_right_platform")
	{
		return m_bottom_right_platformTextures;
	}
	else if (name == "enemy" || name == "bounder" || name == "hunter" || name == "shadowlord")
	{
		return m_enemyTextures;
	}
	else if (name == "rider")
	{
		return m_riderTextures;
	}
	else
	{
		throw std::runtime_error("Invalid sprite name provided to LoadSprites: " + name);
	}

}

// --- Function that returns a single sprite from a sprite map ---
const sf::IntRect& LLGP::AssetRegistry::LoadSprite(const std::string& name, int index)
{
	if (name == "rider")
	{
		// Construct the key name from index
		std::string key = name + std::to_string(index);

		auto it = m_riderTextures.find(key);
		if (it == m_riderTextures.end())
		{
			throw std::out_of_range("No rider texture at index: " + std::to_string(index));
		}

		return it->second;
	}
}

