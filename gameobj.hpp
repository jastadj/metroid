#ifndef CLASS_GAMEOBJ
#define CLASS_GAMEOBJ

#include <SFML\Graphics.hpp>

class GameOBJ
{
protected:

    sf::Vector2f m_Position;
public:

    GameOBJ();
    virtual ~GameOBJ()=0;

    sf::Vector2f getPosition() { return m_Position;}
    void setPosition(sf::Vector2f npos) { m_Position = npos;}

    virtual void update()=0;
    virtual void draw(sf::RenderTarget *trender)=0;
};

#endif // CLASS_GAMEOBJ
