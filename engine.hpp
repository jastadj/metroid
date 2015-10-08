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
enum {MODE_PLAY, MODE_EDIT, MODE_EDIT_PAINT, MODE_TOTAL};

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
    bool initEnemies();
    bool initPlayer();

    //resources
    sf::Texture m_TilesTXT;
    sf::Texture m_RipperTXT;
    std::vector< std::vector<sf::Texture> > m_SamusTXT;
    std::vector< std::vector<sf::Image> > m_SamusIMG;
    sf::Vector2u m_TilesDim;
    std::vector< sf::Sprite* > m_TilesSPR;
    std::vector< sf::Sprite* > m_RipperSPR;
    std::vector<std::vector< sf::Sprite* > > m_SamusSPR;
    sf::Font m_Font;

    //draw
    void drawTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex); //draw tile in tile coordinates
    void drawTileScreenCoord(sf::RenderTarget *tscreen, sf::Vector2f screencoords, unsigned int tindex); //draw tile at screen coordinates
    void drawSuperTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex); // draw a 2x2 tile, using topleft tile as ref

    //loop
    void mainLoop();
    void updateAndDrawGUIobjs(std::vector<GUIobj*> *objlist);
    GUIobj *getGUIclickedOn(std::vector<GUIobj*> *objlist, sf::Vector2f mousePos);
    void updateEnemies();

    //game objects
    Map *m_Map;
    std::vector< GUIobj*> m_GUIobjs;
    Player *m_Player;
    std::vector< Enemy*> m_Enemies;

    //draw functions
    void drawMap();
    void drawEnemies();

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

    void start();

    //get sprites
    std::vector< sf::Sprite* > *getRipperSPR() { return &m_RipperSPR;}
    std::vector< sf::Sprite* > *getSamusSPR(int colorvariant);

    //map
    sf::Vector2i screenToMapCoords(sf::Vector2f mousepos);
    int getMapWidth() { return m_Map->getMapWidth();}
    int getMapHeight() { return m_Map->getMapHeight();}
    const std::vector< std::vector<int> > *getMapData() const { return m_Map->getMapData(); }

    //debug
    void dbg_drawGridBox(int x, int y, sf::Color ncolor = sf::Color(255,0,0,100));
    void dbg_addGridBox(int x, int y);
    std::vector<sf::Vector2f> dbg_gridboxes;
};

#endif // CLASS_ENGINE
