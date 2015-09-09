#include "engine.hpp"

Engine::Engine()
{

    //init screen
    m_Screen = new sf::RenderWindow(sf::VideoMode(CHUNK_SIZE*SCREEN_WIDTH_CHUNKS,
                                                CHUNK_SIZE*SCREEN_HEIGHT_CHUNKS,32), "Metroid");
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

    for(int i = 0; i < 3; i++)
    {
        sf::Sprite *newsprite = new sf::Sprite;
        sf::IntRect srect;
        srect.left = i*CHUNK_SIZE;
        srect.top = 0;
        srect.width = CHUNK_SIZE;
        srect.height = CHUNK_SIZE;
        newsprite->setTexture(m_TilesTXT);
        newsprite->setTextureRect(srect);
        newsprite->setScale( CHUNK_SCALE, CHUNK_SCALE);

        m_TilesSPR.push_back(newsprite);
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

        m_Screen->draw(*m_TilesSPR[0]);

        m_Screen->display();
    }
}
