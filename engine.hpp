#ifndef CLASS_ENGINE
#define CLASS_ENGINE

#include <cstdlib>
#include <SFML\Graphics.hpp>

#define CHUNK_SIZE 16
#define SCREEN_WIDTH_CHUNKS 32
#define SCREEN_HEIGHT_CHUNKS 28

class Engine
{
private:

    sf::RenderWindow *screen;

    void mainLoop();

public:
    Engine();
    ~Engine();
};

#endif // CLASS_ENGINE
