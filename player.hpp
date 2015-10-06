#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "gameobj.hpp"

enum {PLAYER_RUN_NONE, PLAYER_RUN_RIGHT, PLAYER_RUN_LEFT};

class Player: public GameOBJ
{
private:

    sf::Clock debugclock;

    //bool m_OnGround;
    int m_RunDir; // 0 = none, 1 = right, 2 = left

public:
    Player();
    ~Player();

    void update();
    void draw(sf::RenderTarget *trender);

    //bool isOnGround() { return m_OnGround;}

    void runRight(bool nrun);
    void runLeft(bool nrun);

};
#endif // CLASS_PLAYER
