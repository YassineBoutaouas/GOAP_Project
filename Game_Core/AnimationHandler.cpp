#include "AnimationHandler.h"

AnimationHandler::AnimationHandler(const std::string& texturePath, int totalFramesX, int totalFramesY, float pos_x, float pos_y, float scale_x, float scale_y)
	: m_Texture(), m_Sprite(), m_CurrentImage(), m_CellSize_X(), m_CellSize_Y(), m_CurrentIndex_X(0), m_CurrentIndex_Y(0), m_TotalFrames_X(totalFramesX), m_TotalFrames_Y(totalFramesY)
{
	m_Texture.loadFromFile(texturePath);
	m_Sprite.setTexture(m_Texture);

	m_CellSize_X = (m_Texture.getSize().x / totalFramesX);
	m_CellSize_Y = (m_Texture.getSize().y / totalFramesY);
	m_CurrentImage = sf::IntRect(m_CurrentIndex_X, m_CurrentIndex_Y, m_CellSize_X, m_CellSize_Y);
	m_Sprite.setTextureRect(m_CurrentImage);

	m_Sprite.setPosition(pos_x, pos_y);
	m_Sprite.setScale(scale_x, scale_y);
}

void AnimationHandler::Draw(sf::RenderTarget& renderTarget) const {
	renderTarget.draw(m_Sprite);
}

void AnimationHandler::SetFrame(int horizontalIndex, int vertIndex) {
	m_CurrentIndex_X = horizontalIndex;
	m_CurrentIndex_Y = vertIndex;

	if (m_CurrentIndex_X >= m_TotalFrames_X)
		m_CurrentIndex_X = 0;

	if (m_CurrentIndex_Y >= m_TotalFrames_Y)
		m_CurrentIndex_Y = 0;

	m_CurrentImage.left = (m_CellSize_X / 2) * m_CurrentIndex_X;
	m_CurrentImage.top = (m_CellSize_Y / 2) * m_CurrentIndex_Y;

	m_Sprite.setTextureRect(m_CurrentImage);
}

const int AnimationHandler::GetCurrentIndex_X() const {
	return m_CurrentIndex_X;
}

const int AnimationHandler::GetCurrentIndex_Y() const {
	return m_CurrentIndex_Y;
}