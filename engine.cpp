#include "engine.hpp"

Engine::Engine()
{

    //init screen
    m_Screen = new sf::RenderWindow(sf::VideoMode(CHUNK_SIZE*CHUNK_SCALE*SCREEN_WIDTH_CHUNKS,
                                                CHUNK_SIZE*CHUNK_SCALE*SCREEN_HEIGHT_CHUNKS,32), "Metroid");
    initTiles();

    mainLoop();
}

Engine::~Engine()
{

}

bool Engine::initTiles()
{
    //load in texture sprite sheet
    if(!m_TilesTXT.loadFromFile("tiles.png"))
    {
        std::cout << "Error loading tiles.png\n";
        return false;
    }

    unsigned int tsheet_width = m_TilesTXT.getSize().x / CHUNK_SIZE;
    unsigned int tsheet_height = m_TilesTXT.getSize().y / CHUNK_SIZE;

    for(int i = 0; i < tsheet_height; i++)
    {

        for(int n = 0; n < tsheet_width; n++)
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

}

void Engine::mainLoop()
{
    bool quit = false;

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

        drawTile(m_Screen, 1, 1, 0);

        m_Screen->display();
    }
}

void Engine::drawTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex)
{
    if(tindex >= int(m_TilesSPR.size()) )
    {
        std::cout << "drawTile : error, tile index out of bounds.  index=" << tindex << std::endl;
        return;
    }

    m_TilesSPR[tindex]->setPosition(x*CHUNK_SIZE*CHUNK_SCALE, y*CHUNK_SIZE*CHUNK_SCALE);
    tscreen->draw(*m_TilesSPR[tindex]);

}

void Engine::drawTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex)
{
    if(tindex >= int(m_TilesSPR.size()) )
    {
        std::cout << "drawTile : error, tile index out of bounds.  index=" << tindex << std::endl;
        return;
    }
    else if(tindex)

    m_TilesSPR[tindex]->setPosition(x*CHUNK_SIZE*CHUNK_SCALE, y*CHUNK_SIZE*CHUNK_SCALE);
    tscreen->draw(*m_TilesSPR[tindex]);

}
