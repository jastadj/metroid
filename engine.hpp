#ifndef CLASS_ENGINE
#define CLASS_ENGINE

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

#include "defs.hpp"
#include "map.hpp"
#include "guiobj.hpp"
#include "player.hpp"
#include "enemy.hpp"


//metroid map 960x900
enum {MODE_PLAY, MODE_EDIT, MODE_TOTAL};

class Engine
{
private:
    Engine();
    static Engine *onlyinstance;
    sf::RenderWindow *m_Screen;

    int m_Mode;

    //init
    bool initTiles();
    bool initGUIobjs();

    //resources
    sf::Texture m_TilesTXT;
    sf::Vector2u m_TilesDim;
    std::vector< sf::Sprite* > m_TilesSPR;
    sf::Font m_Font;

    //draw
    void drawTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex); //draw tile in tile coordinates
    void drawTileScreenCoord(sf::RenderTarget *tscreen, sf::Vector2f screencoords, unsigned int tindex); //draw tile at screen coordinates
    void drawSuperTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex); // draw a 2x2 tile, using topleft tile as ref

    //loop
    void mainLoop();
    void updateEnemies();

    //game objects
    Map *m_Map;
    std::vector< GUIobj*> m_GUIobjs;
    Player *m_Player;
    std::vector< Enemy*> m_Enemies;

    //draw functions
    void drawMap();
    void drawEnemies();
    sf::Vector2i screenToMapCoords(sf::Vector2f mousepos);

    //map functions
    bool loadMap(std::string mapfile);

    //test
    void testfunct();

public:
    static Engine *getInstance()
    {
        if(onlyinstance == NULL) onlyinstance = new Engine;
        return onlyinstance;
    }
    ~Engine();
};

#endif // CLASS_ENGINE
