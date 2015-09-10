#ifndef CLASS_ENGINE
#define CLASS_ENGINE

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

#define CHUNK_SIZE 8
#define CHUNK_SCALE 2
#define SCREEN_WIDTH_CHUNKS 32*CHUNK_SCALE
#define SCREEN_HEIGHT_CHUNKS 28*CHUNK_SCALE

class Engine
{
private:

    sf::RenderWindow *m_Screen;

    //init
    bool initTiles();

    //resources
    sf::Texture m_TilesTXT;
    std::vector< sf::Sprite* > m_TilesSPR;

    //draw
    void drawTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex); //draw tile in tile coordinates
    void drawSuperTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex); // draw a 2x2 tile, using topleft tile as ref

    //loop
    void mainLoop();

public:
    Engine();
    ~Engine();
};

#endif // CLASS_ENGINE
