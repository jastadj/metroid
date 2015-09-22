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
    float m_Rotation;

    sf::FloatRect m_BoundingBox;

    sf::Clock m_Clock;

public:

    GameOBJ();
    virtual ~GameOBJ()=0;

    sf::Vector2f getPosition() { return m_Position;}
    void setPosition(sf::Vector2f npos) { m_Position = npos;}
    void setPosition(int x, int y) { setPosition(sf::Vector2f(x,y));}
    void setPositionGrid(int x, int y) { setPosition( sf::Vector2f(x*CHUNK_SIZE*CHUNK_SCALE, y*CHUNK_SIZE*CHUNK_SCALE) );}

    virtual void update()=0;
    virtual void updateTransform();
    virtual void draw(sf::RenderTarget *trender)=0;

    //bounding box and collisions
    void drawBoundingBox(sf::RenderTarget *trender);
    bool validPosition();
};

#endif // CLASS_GAMEOBJ
