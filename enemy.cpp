#include "enemy.hpp"

//debug
#include <iostream>
#include <string>

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

/////////////////////////////////////////
// ZOOMER
Zoomer::Zoomer()
{
    m_Sprite = new sf::RectangleShape(sf::Vector2f(16,16));
    m_BoundingBox = sf::FloatRect(0,0,16*CHUNK_SCALE,16*CHUNK_SCALE);
}

Zoomer::~Zoomer()
{

}

void Zoomer::update()
{
    m_Position.x = m_Position.x + 0.5;
    m_Transform = sf::Transform();
    m_Transform.scale(m_Scale);
    m_Transform.translate(m_Position);

    m_BoundingBox.left = m_Position.x;
    m_BoundingBox.top = m_Position.y;
}

void Zoomer::draw(sf::RenderTarget *trender)
{
    trender->draw(*m_Sprite, m_Transform);

    //for debug, draw bounding box
    drawBoundingBox(trender);
}

