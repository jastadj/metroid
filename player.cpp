#include "player.hpp"
#include "engine.hpp"

Player::Player()
{
    //set player bounding box dimensions
    //m_BoundingBox.

    m_RunDir = -1;

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
    updateTransform();

    /*
    //if direction is to the right (0) move positive
    if(!m_Direction) m_Vel.x = RIPPER_MOVE_SPEED;
    else m_Vel.x = -1*RIPPER_MOVE_SPEED;

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
    */

}

void Player::draw(sf::RenderTarget *trender)
{
    //update frame
    if(m_RunDir == 0)
    {
        if(m_Frame > 3) m_Frame = 1;
        else m_Frame++;
        if(m_Frame > 3) m_Frame = 1;
    }
    else if(m_RunDir == 1)
    {
        if(m_Frame < 5) m_Frame = 6;
        else m_Frame++;
        if(m_Frame > 7) m_Frame = 5;
    }
    else if(m_RunDir = -1)
    {
        if(m_Frame >= 0 && m_Frame <= 3) m_Frame = 0;
        else if(m_Frame >=4 && m_Frame <= 7) m_Frame = 4;
        else std::cout << "Error animating player when not running\n";
    }


    std::cout << "rundir = " << m_RunDir << "   -   frame = " << m_Frame << std::endl;

    //get engine ref
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    sf::Sprite *tsprite = NULL;
    tsprite = (*eptr->getSamusSPR())[m_Frame];

    trender->draw(*tsprite, m_Transform);

    drawBoundingBox(trender);
}

void Player::run(int dir)
{
    m_RunDir = dir;

    switch(m_RunDir)
    {
    case -1:
        m_Vel.x = 0;
        break;
    case 0:
        m_Vel.x = PLAYER_MOVE_SPEED;
        break;
    case 1:
        m_Vel.x = -1*PLAYER_MOVE_SPEED;
        break;
    default:
        std::cout << "Error in Player::run(dir) - " << dir << " is an invalid run direction!\n";
        m_RunDir = -1;
        m_Vel.x = 0;
        break;
    }
}

