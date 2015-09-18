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
}

void Zoomer::draw(sf::RenderTarget *trender)
{
    trender->draw(*m_Sprite, m_Transform);

    //sf::FloatRect myrect = m_Transform.transformRect(m_Sprite->getLocalBounds());
    //std::cout << "myrect TL = " << myrect.left << "," << myrect.top << "    BR = " << myrect.left + myrect.width << "," << myrect.top + myrect.height << std::endl;
}

