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

public:
    Bullet(sf::Vector2f spos, sf::Vector2f svel, int stype);
    ~Bullet();

    void update();
    void draw(sf::RenderTarget *trender);

    void explode() { m_Exploding = true;}

};
#endif // CLASS_BULLET
