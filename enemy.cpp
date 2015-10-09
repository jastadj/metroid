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
    sf::Sprite *tsprite = NULL;

    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    trender->draw( *(*eptr->getZoomerSPR())[m_Frame], m_Transform);

    //draw bounding box for debug
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
    if(!m_Direction) m_Vel.x = RIPPER_MOVE_SPEED;
    else m_Vel.x = -1*RIPPER_MOVE_SPEED;

    m_Position += m_Vel;
    updateTransform();

    //if position is not valid, revert adjustment and reverse direction
    if(!validPosition())
    {

        //going right?
        if(m_Direction == 0)
        {
            m_Vel.x = -1*RIPPER_MOVE_SPEED;
        }
        else
        {
            m_Vel.x = RIPPER_MOVE_SPEED;
        }

        //reverse direction
        if(m_Direction == 0) m_Direction = 1;
        else m_Direction = 0;

        updateTransform();
    }

    //determine which sprite to use
    if(!m_Direction) m_Frame = 0;
    else m_Frame = 1;
}


void Ripper::draw(sf::RenderTarget *trender)
{
    sf::Sprite *tsprite = NULL;

    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    trender->draw( *(*eptr->getRipperSPR())[m_Frame], m_Transform);

    //draw bounding box for debug
    drawBoundingBox(trender);
}
