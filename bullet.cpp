#include "bullet.hpp"
#include "engine.hpp"

Bullet::Bullet(sf::Vector2f spos, sf::Vector2f svel, int stype)
{
    m_Position = spos;
    m_Vel = svel;

    m_BulletType = stype;

    m_MaxTimeAlive = 400;
    m_Exploding = false;

    m_BoundingBox = sf::FloatRect(0,0,4*CHUNK_SCALE,5*CHUNK_SCALE);
    m_BoundingBoxOffset = sf::Vector2f(2*CHUNK_SCALE,2*CHUNK_SCALE);
}

Bullet::~Bullet()
{

}


void Bullet::update()
{
    //check bullet life
    if(m_Clock.getElapsedTime().asMilliseconds() > m_MaxTimeAlive && !m_Exploding)
    {
        m_Alive = false;
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
      m_Alive = false;
    }
    else trender->draw( *(*eptr->getBulletSPR())[m_BulletType], m_Transform );

    drawBoundingBox(trender);
}
