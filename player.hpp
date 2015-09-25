#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "gameobj.hpp"

class Player: public GameOBJ
{
private:

    sf::Clock debugclock;
public:
    Player();
    ~Player();

    void update();
    void draw(sf::RenderTarget *trender);

};
#endif // CLASS_PLAYER
