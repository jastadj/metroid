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

    //m_OnGround = false;

    m_RunDir = PLAYER_RUN_NONE;

    //default color variant (green)
    m_ColorVariant = 0;
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

    if(!touchingBottom())
    {
        m_Vel.y += PLAYER_FALL_ACCEL;
        if(m_Vel.y > PLAYER_FALL_TERMINAL_VEL) m_Vel.y = PLAYER_FALL_TERMINAL_VEL;
    }

    m_Position += m_Vel;
    updateTransform();

    if(collidingLeft())
    {
        m_Position.x = oldpos.x;
        updateTransform();

    }
    else if(collidingRight())
    {
        m_Position.x = oldpos.x;
        updateTransform();
    }

    //check for y-axis collision
    if(collidingBottom())
    {
        m_Vel.y = 0;
        while(!touchingBottom())
        {
            m_Position.y--;
            updateTransform();
        }
    }
/*
    m_OnGround = false;

    //before stepping, check if already on ground
    if(!collidingBottom())
    {
        m_Position.y++;
        updateTransform();

        if(collidingBottom()) m_OnGround = true;

        m_Position.y--;
        updateTransform();
    }

    //if(m_OnGround) std::cout << "player on ground\n";

    //apply gravity if not on ground
    if(!m_OnGround)
    {
        m_Vel.y += PLAYER_FALL_ACCEL;
        if(m_Vel.y > PLAYER_FALL_TERMINAL_VEL) m_Vel.y = PLAYER_FALL_TERMINAL_VEL;
    }
    else if( m_Vel.y > 0) m_Vel.y = 0;

    //advance step
    m_Position += m_Vel;
    updateTransform();

    //make collision adjustments to position

    //if(!m_OnGround) std::cout << "not on ground\n";

    //resolve y axis collision
    if(m_Vel.y > 0 && !m_OnGround)
    {
        int tempx = m_Position.x;
        m_Position.x = oldpos.x;

        while(collidingBottom())
        {
            //m_Position.y = oldpos.y;
            m_Position.y--;
            updateTransform();
        }

        m_Position.x = tempx;
    }

    //resolve x axis collision first
    if(collidingRight())
    {

       std::cout << "touching right\n";
        //m_Vel.x = 0;
        //m_Position.x--;
        m_Position.x = oldpos.x;
        updateTransform();
    }

    if(collidingLeft())
    {
        std::cout << "touching left\n";
        //m_Vel.x = 0;
        //m_Position.x++;
        m_Position.x = oldpos.x;

        updateTransform();
    }



*/

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

    //std::cout << "player run dir:" << m_RunDir << std::endl;

    //update frame
    if(m_RunDir == PLAYER_RUN_RIGHT)
    {
        if(m_Frame > 3) m_Frame = 1;
        else m_Frame++;
        if(m_Frame > 3) m_Frame = 1;
    }
    else if(m_RunDir == PLAYER_RUN_LEFT)
    {
        if(m_Frame < 9) m_Frame = 9;
        else m_Frame++;
        if(m_Frame > 11) m_Frame = 9;
    }
    else if(m_RunDir == PLAYER_RUN_NONE)
    {
        //standing to the right
        if(m_Frame >= 0 && m_Frame <= 3) m_Frame = 0;
        //standing to the left
        else if(m_Frame >=8 && m_Frame <= 11) m_Frame = 8;
        else std::cout << "Error animating player when not running\n";
    }

    std::cout << "player frame:" << m_Frame << std::endl;
}

void Player::runRight(bool nrun)
{
    if(nrun)
    {
        m_Vel.x = PLAYER_MOVE_SPEED;
        m_RunDir = PLAYER_RUN_RIGHT;
    }
    else
    {
        m_Vel.x = 0;
        m_RunDir = 0;
    }
}
void Player::runLeft(bool nrun)
{
    if(nrun)
    {
        m_Vel.x = -PLAYER_MOVE_SPEED;
        m_RunDir = PLAYER_RUN_LEFT;
    }
    else
    {
        m_Vel.x = 0;
        m_RunDir = 0;
    }
}

void Player::draw(sf::RenderTarget *trender)
{
    //get engine ref
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    sf::Sprite *tsprite = NULL;
    tsprite = (*eptr->getSamusSPR(m_ColorVariant))[m_Frame];

    trender->draw(*tsprite, m_Transform);

    drawBoundingBox(trender);
}

