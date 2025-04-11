#pragma once

#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

namespace LLGP
{
	class AssetRegistry
	{
	private:
		sf::Texture m_spriteSheet;

		sf::Font m_joustFont;

		std::unordered_map<std::string, sf::IntRect> m_ostrichTextures;
		std::unordered_map<std::string, sf::IntRect> m_storkTextures;
		std::unordered_map<std::string, sf::IntRect> m_vultureTextures;
		std::unordered_map<std::string, sf::IntRect> m_platformTextures;

		void PopulateTextureMap(std::unordered_map<std::string, sf::IntRect>& texture_map,
			int amount,
			const std::string& name,
			int texture_width,
			int texture_height,
			int starting_x_pos,
			int starting_y_pos,
			int padding);

		void StoreTextureMaps();
		void InitFonts();

	public:
		void LoadSpriteSheet();
		sf::Font GetFont();
		const std::unordered_map<std::string, sf::IntRect>& LoadPlayerSprites(int player_id);
		const std::unordered_map<std::string, sf::IntRect>& LoadPlatformSprites();
		sf::Texture& LoadTexture();
	};
}


