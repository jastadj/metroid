#include "player.hpp"
#include "engine.hpp"

Player::Player()
{
    //set player bounding box dimensions
    //m_BoundingBox.

    m_BoundingBox.width = 8*CHUNK_SCALE;
    m_BoundingBox.height = 30*CHUNK_SCALE;

    m_BoundingBoxOffset.x = 10*CHUNK_SCALE;
    m_BoundingBoxOffset.y = 9*CHUNK_SCALE;
}

Player::~Player()
{

}

void Player::update()
{
    //capture old position/velocity
    sf::Vector2f oldpos = m_Position;
    sf::Vector2f oldvel = m_Vel;
    sf::Vector2f coffset;

    //apply gravity
    //m_Vel.y += PLAYER_FALL_ACCEL;
    //if(m_Vel.y > PLAYER_FALL_TERMINAL_VEL) m_Vel.y = PLAYER_FALL_TERMINAL_VEL;

    //advance step
    //updateTransform();

    //check y axis for collision (floor, etc)
    if(touchingBottom() && m_Vel.y > 0)
    {
        m_Vel.y = 0;
        m_Position.y = oldpos.y;
        updateTransform();
    }


    if(touchingRight())
    {
        std::cout << "touching right\n";
        //m_Vel.x = 0;
    }

    if(touchingLeft())
    {
        std::cout << "touching left\n";
        //m_Vel.x = 0;
    }

    //update
    updateTransform();

    //if position is not valid, go back to old position
    /*
    if(!validPosition())
    {
        m_Position = oldpos;

        if(m_Vel.x > 0) m_Vel.x = 0;
        else if(m_Vel.x < 0) m_Vel.x = 0;

        if(m_Vel.y > 0) m_Vel.y = 0;
        else if(m_Vel.y < 0) m_Vel.y = 0;
        updateTransform();
    }
    */


    //update frame
    if(m_Vel.x > 0)
    {
        if(m_Frame > 3) m_Frame = 1;
        else m_Frame++;
        if(m_Frame > 3) m_Frame = 1;
    }
    else if(m_Vel.x < 0)
    {
        if(m_Frame < 5) m_Frame = 6;
        else m_Frame++;
        if(m_Frame > 7) m_Frame = 5;
    }
    else if(m_Vel.x == 0)
    {
        if(m_Frame >= 0 && m_Frame <= 3) m_Frame = 0;
        else if(m_Frame >=4 && m_Frame <= 7) m_Frame = 4;
        else std::cout << "Error animating player when not running\n";
    }
}

void Player::draw(sf::RenderTarget *trender)
{
    //get engine ref
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    sf::Sprite *tsprite = NULL;
    tsprite = (*eptr->getSamusSPR())[m_Frame];

    trender->draw(*tsprite, m_Transform);

    drawBoundingBox(trender);
}

