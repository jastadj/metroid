#ifndef CLASS_BULLET
#define CLASS_BULLET

#include "gameobj.hpp"

enum {BULLET_STANDARD, BULLET_TOTAL};

class Bullet:public GameOBJ
{
private:

    int m_MaxTimeAlive;

    int m_BulletType;

    bool m_Exploding;
    bool m_Dead;

public:
    Bullet(sf::Vector2f spos, sf::Vector2f svel, int stype);
    ~Bullet();

    void update();
    void draw(sf::RenderTarget *trender);

    bool isDead() { return m_Dead;}
    void setDead(bool ndead) { m_Dead = ndead;}
};
#endif // CLASS_BULLET
