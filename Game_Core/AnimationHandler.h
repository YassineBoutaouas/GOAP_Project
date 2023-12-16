#pragma once
#include <SFML/Graphics.hpp>

class AnimationHandler {
private:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	sf::IntRect m_CurrentImage;

	int m_CellSize_X;
	int m_CellSize_Y;

	int m_CurrentIndex_X;
	int m_CurrentIndex_Y;	
	
	int m_TotalFrames_X;
	int m_TotalFrames_Y;

public:
	AnimationHandler(const std::string& texturePath, int totalFramesX, int totalFramesY, float pos_x, float pos_y, float scale_x, float scale_y);

	void Draw(sf::RenderTarget& renderTarget) const;

	void SetFrame(int horizontalIndex, int vertIndex);

	const int GetCurrentIndex_X() const;
	const int GetCurrentIndex_Y() const;
};