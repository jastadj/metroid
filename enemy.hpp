#ifndef CLASS_ENEMY
#define CLASS_ENEMY

#include "gameobj.hpp"

class Enemy: public GameOBJ
{
protected:

public:
    Enemy();
    ~Enemy();
};

class Zoomer:public Enemy
{
private:

    sf::RectangleShape *m_Sprite;

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
