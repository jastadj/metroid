#include "enemy.hpp"
#include "engine.hpp"

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

/////////////////////////////////////////
// RIPPER
Ripper::Ripper()
{
    //use rotation 0 = to the right, 90 = to the left
    m_BoundingBox = sf::FloatRect(0,0,16*CHUNK_SCALE,8*CHUNK_SCALE);
}

Ripper::~Ripper()
{

}

void Ripper::update()
{
    m_Transform = sf::Transform();
    m_Transform.scale(m_Scale);
    m_Transform.translate(m_Position);

    sf::Vector2f testpoint = m_Transform.transformPoint(0,0);
    m_BoundingBox.left = testpoint.x;
    m_BoundingBox.top = testpoint.y;
}

void Ripper::draw(sf::RenderTarget *trender)
{
    sf::Sprite *tsprite = NULL;

    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    //determine which sprite to use
    if(m_Rotation == 0)
    {
        trender->draw( *(*eptr->getRipperSPR())[0], m_Transform);
    }

    drawBoundingBox(trender);
}
