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

    m_Jumping = false;
    m_AimingUp = false;

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

    //if player is not touching the ground
    if(!touchingBottom())
    {
        //is player currently in a jump state?  if so, keep jump vel going
        if(m_JumpClock.getElapsedTime().asMilliseconds() <= PLAYER_JUMP_TIMEOUT_MS && m_Jumping)
        {
            m_Vel.y = -PLAYER_JUMP_VEL;
        }
        //if not jumping, then player is falling, apply gravity
        else
        {
            m_Vel.y += PLAYER_FALL_ACCEL;
            if(m_Vel.y > PLAYER_FALL_TERMINAL_VEL) m_Vel.y = PLAYER_FALL_TERMINAL_VEL;
        }

    }
    //else player is on the ground

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
        m_Jumping = false;
        while(!touchingBottom())
        {
            m_Position.y--;
            updateTransform();
        }
    }

    if(collidingTop())
    {
        m_Vel.y = 0;
        while(!touchingTop())
        {
            m_Position.y++;
            updateTransform();
        }
    }

    //update
    updateTransform();

    //update frame
    //player is running to the right
    if(m_RunDir == PLAYER_RUN_RIGHT)
    {
        //if not touching the ground, set frame to jump
        if(!touchingBottom())
        {
            m_Frame = 4;
        }
        else
        {
            if(m_Frame > 3) m_Frame = 1;
            else m_Frame++;
            if(m_Frame > 3) m_Frame = 1;
        }

    }
    //player is running to the left
    else if(m_RunDir == PLAYER_RUN_LEFT)
    {
        if(!touchingBottom())
        {
            m_Frame = 4 + PLAYER_FRAMES_IN_SET;
        }
        else
        {
            if(m_Frame < 1 + PLAYER_FRAMES_IN_SET) m_Frame = 1 + PLAYER_FRAMES_IN_SET;
            else m_Frame++;
            if(m_Frame > 3 + PLAYER_FRAMES_IN_SET) m_Frame = 1 + PLAYER_FRAMES_IN_SET;
        }

    }
    //player is standing still
    else if(m_RunDir == PLAYER_RUN_NONE)
    {
        //if not touching the ground, set frame to jump
        if(!touchingBottom())
        {
            if(facingRight()) m_Frame = 4;
            else m_Frame = 4 + PLAYER_FRAMES_IN_SET;
        }
        //else player is touching the ground
        else
        {
            if(facingRight())
            {
                //if player is aiming up
                if(m_AimingUp) m_Frame = 12;
                //if not, just standing frame
                else m_Frame = 0;
            }
            //else player is facing left
            else
            {
                //if player is aiming up
                if(m_AimingUp) m_Frame = PLAYER_FRAMES_IN_SET + 12;
                //if not, just standing frame
                else m_Frame = PLAYER_FRAMES_IN_SET;
            }
        }
    }

    //update bullet origin
    if(m_AimingUp)
    {
        if(facingRight()) m_BulletOriginOffset = sf::Vector2f(11,2);
        else m_BulletOriginOffset = sf::Vector2f(9,2);
    }
    else if(facingRight())
    {
        m_BulletOriginOffset = sf::Vector2f(20, 13);
    }
    else m_BulletOriginOffset = sf::Vector2f(0,13);

    //std::cout << "player frame:" << m_Frame << std::endl;
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

bool Player::facingRight()
{
    if(m_Frame >= 0 && m_Frame < PLAYER_FRAMES_IN_SET) return true;
    else return false;
}

bool Player::facingLeft()
{
    if(!facingRight()) return true;
    else return false;
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

Bullet *Player::fireBullet()
{
    sf::Vector2f bvel;

    if(m_AimingUp) bvel.y = -PLAYER_BULLET_0_SPEED;
    else if(facingRight()) bvel.x = PLAYER_BULLET_0_SPEED;
    else bvel.x = -PLAYER_BULLET_0_SPEED;

    //std::cout << "bullet vel = " << bvel.x << "," << bvel.y << std::endl;
    Bullet *newbullet = new Bullet( m_Position + m_BulletOriginOffset, bvel, 0);

    return newbullet;

}

void Player::jump()
{

    if(touchingBottom())
    {
        m_JumpClock.restart();
        m_Jumping = true;
        m_Vel.y = -PLAYER_JUMP_VEL;
    }
}
