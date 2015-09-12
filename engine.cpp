#include "engine.hpp"

Engine::Engine()
{
    //init pointers
    m_Map = NULL;

    //init vars
    m_Mode = MODE_PLAY;

    //init screen
    std::cout << "Initializing render window...\n";
    m_Screen = new sf::RenderWindow(sf::VideoMode(CHUNK_SIZE*CHUNK_SCALE*SCREEN_WIDTH_CHUNKS,
                                                CHUNK_SIZE*CHUNK_SCALE*SCREEN_HEIGHT_CHUNKS,32), "Metroid");
    initTiles();
    initGUIobjs();

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

bool Engine::initGUIobjs()
{
    GUIobj *newobj = new WindowPane(sf::Vector2f(CHUNK_SIZE*CHUNK_SCALE*SCREEN_WIDTH_CHUNKS,
                                                CHUNK_SIZE*CHUNK_SCALE*4));
    newobj->setVisible(true);
    newobj->setMovable(true);
    m_GUIobjs.push_back(newobj);

    newobj = new SpriteButton(m_TilesSPR[2]);
    newobj->setVisible(true);
    newobj->setPosition(90,90);
    m_GUIobjs.push_back(newobj);

    return true;
}

void Engine::mainLoop()
{
    bool quit = false;

    //create viewport
    sf::View view;
    sf::Vector2f viewcenter(16*CHUNK_SIZE*CHUNK_SCALE, 15*CHUNK_SIZE*CHUNK_SCALE);
    view.setSize(CHUNK_SIZE*CHUNK_SCALE*SCREEN_WIDTH_CHUNKS, CHUNK_SIZE*CHUNK_SCALE*SCREEN_HEIGHT_CHUNKS);

    GUIobj *guiselector = NULL;

    while(!quit)
    {
        view.setCenter(viewcenter);
        m_Screen->setView(view);

        m_Screen->clear();

        sf::Event event;
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*m_Screen));

        //update gui objects being manipulated
        if(guiselector != NULL)
        {
            if(guiselector->movable())
                guiselector->setPosition( mousePos - guiselector->m_ClickedOffset + sf::Vector2f(2,2));

            //if left mouse is no longer being held down, deselect gui obj
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                //if mouse released over gui obj (assume clicked , ie button)
                if(guiselector->mouseOver(mousePos))
                {
                    guiselector->doClicked();
                }
                guiselector = NULL;
            }
        }


        while(m_Screen->pollEvent(event) )
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
                //move viewport around
                else if(event.key.code == sf::Keyboard::W) viewcenter.y -= CHUNK_SIZE*CHUNK_SCALE;
                else if(event.key.code == sf::Keyboard::S) viewcenter.y += CHUNK_SIZE*CHUNK_SCALE;
                else if(event.key.code == sf::Keyboard::A) viewcenter.x -= CHUNK_SIZE*CHUNK_SCALE;
                else if(event.key.code == sf::Keyboard::D) viewcenter.x += CHUNK_SIZE*CHUNK_SCALE;
                else if(event.key.code == sf::Keyboard::F1) m_GUIobjs[0]->setVisible( !m_GUIobjs[0]->visible());
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    //check for gui objs if mouse clicked on
                    for(int i = 0; i < int(m_GUIobjs.size()); i++)
                    {
                        if(!m_GUIobjs[i]->visible()) continue;
                        if( m_GUIobjs[i]->mouseOver(mousePos))
                        {
                            guiselector = m_GUIobjs[i];

                            guiselector->m_ClickedOffset = mousePos - sf::Vector2f(guiselector->getRect().left,
                                                                   guiselector->getRect().top);
                            break;
                        }
                    }
                }
            }//end mouse button event
        }//end event handling

        drawMap();

        //draw ui
        m_Screen->setView(m_Screen->getDefaultView());

        for(int i = 0; i < int(m_GUIobjs.size()); i++)
        {
            if( !m_GUIobjs[i]->visible() ) continue;
            m_GUIobjs[i]->update(mousePos);
            m_GUIobjs[i]->draw(m_Screen);
        }

        m_Screen->display();
    }
}

void Engine::drawTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex)
{
    //if tile == -1, treat as a blank tile and ignore
    if(int(tindex) == -1) return;

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

    for(int i = 0; i < int(m_Map->getMapDims().y); i++)
    {
        for(int n = 0; n < int(m_Map->getMapDims().x); n++)
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

///////////////////////
// debug
void Engine::testfunct()
{
    std::cout << "Test functioned executed...\n";
}
