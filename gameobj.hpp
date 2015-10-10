#ifndef CLASS_GAMEOBJ
#define CLASS_GAMEOBJ

#include <SFML\Graphics.hpp>
#include <vector>
#include "defs.hpp"

class GameOBJ
{
protected:

    sf::Vector2f m_Position;
    sf::Transform m_Transform;
    sf::Vector2f m_Scale;
    sf::Vector2f m_Vel;
    float m_Rotation;
    sf::Vector2f m_RotationCenter;

    sf::FloatRect m_BoundingBox;
    sf::Vector2f m_BoundingBoxOffset;

    int m_Frame;

    sf::Clock m_Clock;

public:

    GameOBJ();
    virtual ~GameOBJ()=0;

    sf::Vector2f getPosition() { return m_Position;}
    sf::Vector2f getVelocity() { return m_Vel;}
    void setPosition(sf::Vector2f npos) { m_Position = npos;}
    void setPosition(int x, int y) { setPosition(sf::Vector2f(x,y));}
    void setPositionGrid(int x, int y) { setPosition( sf::Vector2f(x*CHUNK_SIZE*CHUNK_SCALE, y*CHUNK_SIZE*CHUNK_SCALE) );}
    void setRotation(float nrot);
    void setRotationCenter(sf::Vector2f nrotcenter) { m_RotationCenter = nrotcenter;}
    float getRotation() { return m_Rotation;}
    void setVelocity(float x, float y) { m_Vel = sf::Vector2f(x,y);}
    void setVelocity(sf::Vector2f nvel) { m_Vel = nvel;}
    void setVelocityX(float xvel) { m_Vel.x = xvel;}
    void setVelocityY(float yvel) { m_Vel.y = yvel;}

    virtual void update()=0;
    virtual void updateTransform();
    virtual void draw(sf::RenderTarget *trender)=0;

    //bounding box and collisions
    void drawBoundingBox(sf::RenderTarget *trender);
    bool validPosition();
    bool collidingBottom();
    bool collidingTop();
    bool collidingLeft();
    bool collidingRight();
    bool touchingBottom();
    bool touchingTop();
    bool touchingRight();
    bool touchingLeft();
};

#endif // CLASS_GAMEOBJ
