#pragma once

#include <unordered_map>
#include <string>
#include <random>
#include <SFML/Graphics.hpp>

namespace LLGP
{
	class AssetRegistry
	{
	private:
		// Textures
		sf::Texture m_spriteSheet;

		// Fonts
		sf::Font m_joustFont;

		// Maps
		std::unordered_map<std::string, sf::IntRect> m_ostrichTextures;
		std::unordered_map<std::string, sf::IntRect> m_storkTextures;
		std::unordered_map<std::string, sf::IntRect> m_enemyTextures;
		std::unordered_map<std::string, sf::IntRect> m_riderTextures;
		std::unordered_map<std::string, sf::IntRect> m_bottom_platformTextures;
		std::unordered_map<std::string, sf::IntRect> m_top_left_platformTextures;
		std::unordered_map<std::string, sf::IntRect> m_bottom_left_platformTextures;
		std::unordered_map<std::string, sf::IntRect> m_bottom_middle_platformTextures;
		std::unordered_map<std::string, sf::IntRect> m_top_middle_platformTextures;
		std::unordered_map<std::string, sf::IntRect> m_middle_right_platformTextures;
		std::unordered_map<std::string, sf::IntRect> m_bottom_right_platformTextures;
		std::unordered_map<std::string, sf::IntRect> m_top_right_platformTextures;

		// Functions
		void PopulateTextureMap(std::unordered_map<std::string, sf::IntRect>& texture_map,
			int amount,
			const std::string& name,
			int texture_width,
			int texture_height,
			int starting_x_pos,
			int starting_y_pos,
			int padding);

		void AddSpriteToMap(std::unordered_map<std::string, sf::IntRect>& texture_map, const std::string& name, int xPos, int yPos, int width, int height);
		void StoreTextureMaps();
		void InitFonts();

	public:
		// Functions
		void LoadSpriteSheet();
		const std::unordered_map<std::string, sf::IntRect>& LoadSprites(sf::String name);
		const sf::IntRect& LoadSprite(const std::string& name, int index);
		sf::Texture& LoadTexture();

		// Accessor functions
		sf::Font GetFont();
	};
}


