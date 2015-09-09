#include "engine.hpp"

Engine::Engine()
{

    //init screen
    screen = new sf::RenderWindow(sf::VideoMode(CHUNK_SIZE*SCREEN_WIDTH_CHUNKS,
                                                CHUNK_SIZE*SCREEN_HEIGHT_CHUNKS,32), "Metroid");


    mainLoop();
}

Engine::~Engine()
{

}

void Engine::mainLoop()
{
    bool quit = false;

    while(!quit)
    {
        screen->clear();

        sf::Event event;

        while(screen->pollEvent(event) )
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
            }
        }


        screen->display();
    }
}
