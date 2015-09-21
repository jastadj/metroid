#include "gameobj.hpp"
#include <iostream>
#include <string>

GameOBJ::GameOBJ()
{
    m_Scale = sf::Vector2f(CHUNK_SCALE, CHUNK_SCALE);
    m_Rotation = 0;
}

GameOBJ::~GameOBJ()
{

}

void GameOBJ::drawBoundingBox(sf::RenderTarget *trender)
{
    sf::RectangleShape bbox( sf::Vector2f(m_BoundingBox.width-2, m_BoundingBox.height-2) );
    bbox.setOutlineThickness(1);

    bbox.setPosition(m_BoundingBox.left+1, m_BoundingBox.top+1);
    bbox.setFillColor(sf::Color::Transparent);
    bbox.setOutlineColor(sf::Color::Green);

    trender->draw(bbox);
}
