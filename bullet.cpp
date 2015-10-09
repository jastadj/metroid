#include "bullet.hpp"
#include "engine.hpp"

Bullet::Bullet(sf::Vector2f spos, sf::Vector2f svel, int stype)
{
    m_Position = spos;
    m_Vel = svel;

    m_BulletType = stype;

    m_MaxTimeAlive = 400;
    m_Dead = false;
    m_Exploding = false;
}

Bullet::~Bullet()
{

}


void Bullet::update()
{
    //check bullet life
    if(m_Clock.getElapsedTime().asMilliseconds() > m_MaxTimeAlive && !m_Exploding)
    {
        m_Dead = true;
        return;
    }

    m_Position += m_Vel;
    updateTransform();

    //check if hitting map
    if(!validPosition())
    {
        m_Exploding = true;
    }

}

void Bullet::draw(sf::RenderTarget *trender)
{

    //engine pointer
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    if(m_Exploding)
    {
      trender->draw( *(*eptr->getBulletSPR())[m_BulletType+1], m_Transform );
      m_Dead = true;
    }
    else trender->draw( *(*eptr->getBulletSPR())[m_BulletType], m_Transform );
}
