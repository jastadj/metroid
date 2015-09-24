#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "gameobj.hpp"

class Player: public GameOBJ
{
private:

    int m_RunDir; // -1 = no run, 0 = run right, 1 = run left

    sf::Clock debugclock;
public:
    Player();
    ~Player();

    void update();
    void draw(sf::RenderTarget *trender);

    void run(int dir);
    void jump();
};
#endif // CLASS_PLAYER
