#include "engine.hpp"

Engine::Engine()
{
    //init pointers
    m_Map = NULL;

    //init screen
    std::cout << "Initializing render window...\n";
    m_Screen = new sf::RenderWindow(sf::VideoMode(CHUNK_SIZE*CHUNK_SCALE*SCREEN_WIDTH_CHUNKS,
                                                CHUNK_SIZE*CHUNK_SCALE*SCREEN_HEIGHT_CHUNKS,32), "Metroid");
    initTiles();

    loadMap("map.dat");

    std::cout << "Starting main loop...\n";
    mainLoop();
}

Engine::~Engine()
{

}

bool Engine::initTiles()
{
    std::cout << "Initializing tiles...\n";

    //load in texture sprite sheet
    if(!m_TilesTXT.loadFromFile("tiles.png"))
    {
        std::cout << "Error loading tiles.png\n";
        return false;
    }

    m_TilesDim.x = m_TilesTXT.getSize().x / CHUNK_SIZE;
    m_TilesDim.y = m_TilesTXT.getSize().y / CHUNK_SIZE;

    for(int i = 0; i < int(m_TilesDim.y); i++)
    {

        for(int n = 0; n < int(m_TilesDim.x); n++)
        {
            sf::Sprite *newsprite = new sf::Sprite;
            sf::IntRect srect;
            srect.left = n*CHUNK_SIZE;
            srect.top = i*CHUNK_SIZE;
            srect.width = CHUNK_SIZE;
            srect.height = CHUNK_SIZE;
            newsprite->setTexture(m_TilesTXT);
            newsprite->setTextureRect(srect);
            newsprite->setScale( CHUNK_SCALE, CHUNK_SCALE);

            m_TilesSPR.push_back(newsprite);
        }
    }

    std::cout << "Loaded " << m_TilesSPR.size() << " tiles successfully.\n";
    return true;
}

void Engine::mainLoop()
{
    bool quit = false;

    sf::View view;
    view.setCenter(0,0);

    m_Screen->setView(view);

    while(!quit)
    {
        m_Screen->clear();

        sf::Event event;

        while(m_Screen->pollEvent(event) )
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
            }
        }

        drawMap();

        m_Screen->display();
    }
}

void Engine::drawTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex)
{
    //if tile == -1, treat as a blank tile and ignore
    if(tindex == -1) return;

    if(tindex >= unsigned(m_TilesSPR.size()) )
    {
        std::cout << "drawTile : error, tile index out of bounds.  index=" << tindex << std::endl;
        return;
    }

    m_TilesSPR[tindex]->setPosition(x*CHUNK_SIZE*CHUNK_SCALE, y*CHUNK_SIZE*CHUNK_SCALE);
    tscreen->draw(*m_TilesSPR[tindex]);

}

void Engine::drawSuperTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex)
{
    if(tindex >= unsigned(m_TilesSPR.size()) )
    {
        std::cout << "drawTile : error, tile index out of bounds.  index=" << tindex << std::endl;
        return;
    }
    else if(tindex + m_TilesDim.x + 1 >= unsigned(m_TilesSPR.size()) )
    {
        std::cout << "drawSuperTile : error, tile index for 2x2 tiles out of bounds.  top left index=" << tindex << std::endl;
        return;
    }

    //top left sprite
    drawTile(tscreen, x, y, tindex);
    //top right sprite
    drawTile(tscreen, x+1, y, tindex+1);
    //bottom left sprite
    drawTile(tscreen, x, y+1, tindex+m_TilesDim.x);
    //bottom right sprite
    drawTile(tscreen, x+1, y+1, tindex+m_TilesDim.x+1);

}

void Engine::drawMap()
{
    if(m_Map == NULL)
    {
        std::cout << "Error drawing map.  No valid map loaded!\n";
        return;
    }

    for(int i = 0; i < m_Map->getMapDims().y; i++)
    {
        for(int n = 0; n < m_Map->getMapDims().x; n++)
        {
            drawTile(m_Screen, n, i, m_Map->getTileAt(n,i));
        }
    }
}

bool Engine::loadMap(std::string mapfile)
{
    std::cout << "Loading map from file : " << mapfile << std::endl;

    if(m_Map != NULL)
    {
        std::cout << "Deleting current map...\n";
        delete m_Map;
    }
    m_Map = new Map;
    if(!m_Map->loadMapFile(mapfile))
    {
        std::cout << "Deleting map.\n";
        delete m_Map;
        std::cout << "Setting map to NULL.\n";
        m_Map = NULL;
        return false;
    }

    std::cout << "Loaded map from file " << mapfile << " successfully.\n";
    return true;
}
