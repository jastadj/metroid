#include "player.hpp"
#include "engine.hpp"

Player::Player()
{
    //set player bounding box dimensions
    //m_BoundingBox.

    m_RunDir = -1;
}

Player::~Player()
{

}

void Player::update()
{
    updateTransform();

}

void Player::draw(sf::RenderTarget *trender)
{
    //update frame
    if(m_RunDir == 0)
    {
        m_Frame++;
        if(m_Frame > 3) m_Frame = 1;
    }
    else if(m_RunDir == 1)
    {
        m_Frame++;
        if(m_Frame > 7) m_Frame = 5;
    }
    else if(m_RunDir = -1)
    {
        if(m_Frame >= 0 && m_Frame <= 3) m_Frame = 0;
        else if(m_Frame >=4 && m_Frame <= 7) m_Frame = 4;
        else std::cout << "Error animating player when not running\n";
    }
    //get engine ref
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    sf::Sprite *tsprite = NULL;
    tsprite = (*eptr->getSamusSPR())[m_Frame];

    trender->draw(*tsprite, m_Transform);
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

