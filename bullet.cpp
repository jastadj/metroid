#include "bullet.hpp"
#include "engine.hpp"

Bullet::Bullet(sf::Vector2f spos, sf::Vector2f svel, int stype)
{
    m_Position = spos;
    m_Vel = svel;

    m_BulletType = stype;

    m_MaxTimeAlive = 1000;
    m_Dead = false;
}

Bullet::~Bullet()
{

}


void Bullet::update()
{
    //check bullet life
    if(m_Clock.getElapsedTime().asMilliseconds() > m_MaxTimeAlive)
    {
        m_Dead = true;
        return;
    }

    m_Position += m_Vel;
    updateTransform();

}

void Bullet::draw(sf::RenderTarget *trender)
{

    //engine pointer
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    trender->draw( *(*eptr->getBulletSPR())[m_BulletType], m_Transform );
}
