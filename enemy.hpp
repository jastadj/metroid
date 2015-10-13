#ifndef CLASS_ENEMY
#define CLASS_ENEMY

#include "gameobj.hpp"

class Enemy: public GameOBJ
{
protected:

    int m_Direction;
    int m_Health;


public:
    Enemy();
    ~Enemy();

    void setDirection(int ndir) { m_Direction = ndir;}
    int getDirection() { return m_Direction;}

};

class Zoomer:public Enemy
{
private:

public:
    Zoomer();
    ~Zoomer();

    void update();
    void draw(sf::RenderTarget *trender);
};

class Ripper:public Enemy
{
private:

public:
    Ripper();
    ~Ripper();

    void update();
    void draw(sf::RenderTarget *trender);
};
#endif // CLASS_ENEMY
