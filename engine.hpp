#ifndef CLASS_ENGINE
#define CLASS_ENGINE

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

#include "map.hpp"
#include "guiobj.hpp"

#define CHUNK_SIZE 8
#define CHUNK_SCALE 2
#define SCREEN_WIDTH_CHUNKS 16*2
#define SCREEN_HEIGHT_CHUNKS 15*2

//metroid map 960x900
enum {MODE_PLAY, MODE_EDIT, MODE_TOTAL};

class Engine
{
private:

    sf::RenderWindow *m_Screen;

    int m_Mode;

    //init
    bool initTiles();
    bool initGUIobjs();

    //resources
    sf::Texture m_TilesTXT;
    sf::Vector2u m_TilesDim;
    std::vector< sf::Sprite* > m_TilesSPR;

    //draw
    void drawTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex); //draw tile in tile coordinates
    void drawTileScreenCoord(sf::RenderTarget *tscreen, sf::Vector2f screencoords, unsigned int tindex); //draw tile at screen coordinates
    void drawSuperTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex); // draw a 2x2 tile, using topleft tile as ref

    //loop
    void mainLoop();

    //game objects
    Map *m_Map;
    std::vector< GUIobj*> m_GUIobjs;

    //draw functions
    void drawMap();
    sf::Vector2i screenToMapCoords(sf::Vector2f mousepos);

    //map functions
    bool loadMap(std::string mapfile);

    //test
    void testfunct();

public:
    Engine();
    ~Engine();
};

#endif // CLASS_ENGINE
