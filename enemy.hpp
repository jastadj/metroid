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

public:
    Zoomer();
    ~Zoomer();

    void update();
    void draw(sf::RenderTarget *trender);
};
#endif // CLASS_ENEMY
