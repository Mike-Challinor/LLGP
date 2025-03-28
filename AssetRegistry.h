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

		std::unordered_map<std::string, sf::IntRect> m_ostrichTextures;
		std::unordered_map<std::string, sf::IntRect> m_storkTextures;
		std::unordered_map<std::string, sf::IntRect> m_vultureTextures;

		void PopulateTextureMap(std::unordered_map<std::string, sf::IntRect>& texture_map,
			int amount,
			const std::string& name,
			int texture_width,
			int texture_height,
			int starting_x_pos,
			int starting_y_pos,
			int padding);

		void StoreTextureMaps();

	public:
		void LoadSpriteSheet();
		const std::unordered_map<std::string, sf::IntRect>& LoadPlayerSprites(int player_id);
		sf::Texture& LoadTexture();
	};
}


