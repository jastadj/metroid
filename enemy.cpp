#include "enemy.hpp"
#include "engine.hpp"

//debug
#include <iostream>
#include <string>

Enemy::Enemy()
{
    m_Direction = 0;
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
    updateTransform();
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
    //use direction 0 = to the right, 90 = to the left
    m_BoundingBox = sf::FloatRect(0,0,16*CHUNK_SCALE,8*CHUNK_SCALE);
}

Ripper::~Ripper()
{

}

void Ripper::update()
{
    //if direction is to the right (0) move positive
    if(!m_Direction) m_Vel.x = 1;
    else m_Vel.x = -1;

    updateTransform();

    //if position is not valid, revert adjustment and reverse direction
    if(!validPosition())
    {

        //going right?
        if(m_Direction == 0)
        {
            m_Vel.x = -1;
        }
        else
        {
            m_Vel.x = 1;
        }

        //reverse direction
        if(m_Direction == 0) m_Direction = 1;
        else m_Direction = 0;

        updateTransform();
    }
}


void Ripper::draw(sf::RenderTarget *trender)
{
    sf::Sprite *tsprite = NULL;

    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    //determine which sprite to use
    if(!m_Direction)
    {
        trender->draw( *(*eptr->getRipperSPR())[0], m_Transform);
    }
    else trender->draw( *(*eptr->getRipperSPR())[1], m_Transform);

    //draw bounding box for debug
    drawBoundingBox(trender);
}
