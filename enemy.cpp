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
    m_Transform = sf::Transform();
    m_Transform.scale(m_Scale);
    m_Transform.translate(m_Position);


    sf::Vector2f testpoint = m_Transform.transformPoint(0,0);
    m_BoundingBox.left = testpoint.x;
    m_BoundingBox.top = testpoint.y;
}

void Zoomer::draw(sf::RenderTarget *trender)
{
    trender->draw(*m_Sprite, m_Transform);

    //for debug, draw bounding box
    drawBoundingBox(trender);
}

