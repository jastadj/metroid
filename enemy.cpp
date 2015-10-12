#include "enemy.hpp"
#include "engine.hpp"

#include <cmath>

//debug
#include <iostream>
#include <string>

Enemy::Enemy()
{
    m_Direction = 0;
    m_Alive = true;
    m_Health = 1;
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
    int dir_to_check = 0;  // 1 = top, 2 = right, 3 = bottom, 4 = left
    bool needs_rotation = false;

    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    //update zoomer velocity based on rotation and move direction
    m_Vel.x = cos(m_Rotation * PI / 180) * ZOOMER_MOVE_SPEED;
    m_Vel.y = sin(m_Rotation * PI / 180) * ZOOMER_MOVE_SPEED;

    //trim off tiny values to 0
    if(fabs(m_Vel.x) < 0.00001) m_Vel.x = 0;
    if(fabs(m_Vel.y) < 0.00001) m_Vel.y = 0;

    //inverse velocity if direction is clockwise
    if(m_Direction == 1)
    {
        m_Vel.x = m_Vel.x * (-1);
        m_Vel.y = m_Vel.y * (-1);
    }

    //determine direction to check for collision based on velocity direction
    if(m_Vel.x > 0) dir_to_check = 2;
    else if(m_Vel.x < 0) dir_to_check = 4;
    else if(m_Vel.y > 0) dir_to_check = 3;
    else if(m_Vel.y < 0) dir_to_check = 1;

    //std::cout << "zoomer vel = " << m_Vel.x << "," << m_Vel.y << std::endl;

    //make step
    m_Position += m_Vel;
    updateTransform();

    //if zoomer is moving left, check collision left
    if(dir_to_check == 4)
    {
        //if colliding left
        if(collidingLeft())
        {
            m_Vel = sf::Vector2f(0,0);

            //push out to the right
            while(!touchingLeft())
            {
                m_Position.x += 1;
                updateTransform();
            }

            //flag for rotation
            needs_rotation = true;
        }

        else //check that it is still touching the floor
        {
            if(m_Direction == 0 && !touchingTop()) needs_rotation = true;
            else if(m_Direction == 1 && !touchingBottom()) needs_rotation = true;

        }
    }
    //check right
    else if(dir_to_check == 2)
    {
        //if colliding left
        if(collidingRight())
        {
            m_Vel = sf::Vector2f(0,0);

            //push out to the left
            while(!touchingRight())
            {
                m_Position.x -= 1;
                updateTransform();
            }

            //flag for rotation
            needs_rotation = true;
        }

        else //check that it is still touching the floor
        {
            if(m_Direction == 1 && !touchingTop()) needs_rotation = true;
            else if(m_Direction == 0 && !touchingBottom()) needs_rotation = true;

        }
    }
    //check top
    else if(dir_to_check == 1)
    {
        //if colliding top
        if(collidingTop())
        {
            m_Vel = sf::Vector2f(0,0);

            //push out to the bottom
            while(!touchingTop())
            {
                m_Position.y += 1;
                updateTransform();
            }

            //flag for rotation
            needs_rotation = true;
        }

        else //check that it is still touching the floor
        {
            if(m_Direction == 0 && !touchingRight())
            {
                m_Position.y += 1;
                updateTransform();

                needs_rotation = true;
            }
            else if(m_Direction == 1 && !touchingLeft())
            {

                m_Position.y += 1;
                //super buggy shit
                //m_Position.x -= 5;
                updateTransform();

                needs_rotation = true;
            }

        }
    }
    //check bottom
    else if(dir_to_check == 3)
    {
        //if colliding bottom
        if(collidingBottom())
        {
            m_Vel = sf::Vector2f(0,0);

            //push out to the top
            while(!touchingBottom())
            {
                m_Position.y -= 1;
                updateTransform();
            }

            //flag for rotation
            needs_rotation = true;
        }

        else //check that it is still touching the floor
        {
            if(m_Direction == 1 && !touchingRight()) needs_rotation = true;
            else if(m_Direction == 0 && !touchingLeft()) needs_rotation = true;

        }
    }

    // if rotation is needed, handle based on move direction of zoomer
    if(needs_rotation)
    {
        //if move direction is 0 (moving counter clockwise, - 90 deg)
        if(m_Direction == 0) setRotation( getRotation() - 90);
        else setRotation( getRotation() + 90);
    }

    //check if bullets are hitting it
    std::vector< Bullet*> *bullets = eptr->getBullets();
    //go through all bullets
    for(int i = int(bullets->size()-1); i >= 0; i--)
    {
        //if enemy bounding box intersects bullet's bounding box, it's a hit
        if( m_BoundingBox.intersects( *(*bullets)[i]->getBoundingBox()) )
        {
            std::cout << "hit\n";
            //kill bullet, need to add explosion frame
            (*bullets)[i]->setDead(true);

            //decrease enemy health
            m_Health--;
        }
    }

    //check if enemy is dead
    if(m_Health <= 0) m_Alive = false;

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
