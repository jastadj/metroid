#include "engine.hpp"
#include <sstream>

Engine *Engine::onlyinstance = NULL;

Engine::Engine()
{

}

void Engine::start()
{
    //init pointers
    m_Map = NULL;
    m_Player = NULL;

    //init vars
    m_Mode = MODE_PLAY;

    //init screen
    std::cout << "Initializing render window...\n";
    m_Screen = new sf::RenderWindow(sf::VideoMode(CHUNK_SIZE*CHUNK_SCALE*SCREEN_WIDTH_CHUNKS,
                                                CHUNK_SIZE*CHUNK_SCALE*SCREEN_HEIGHT_CHUNKS,32), "Metroid");
    m_Screen->setFramerateLimit(FRAMERATE);
    //m_Screen->setKeyRepeatEnabled(false);

    initTiles();
    initGUIobjs();
    initEnemies();
    initPlayer();

    //init font
    std::cout << "Loading font...\n";
    if(!m_Font.loadFromFile("font.ttf"))
        std::cout << "Error loading font.ttf\n";


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
    newobj->setVisible(false);
    newobj->setMovable(true);
    m_GUIobjs.push_back(newobj);

    newobj = new SpriteButton(m_TilesSPR[2]);
    newobj->setVisible(true);
    newobj->setPosition(10,10);

    m_GUIobjs.back()->addChild(newobj);

    return true;
}

bool Engine::initEnemies()
{
    std::cout << "Initializing enemies...\n";
    //load ripper
    if(!m_RipperTXT.loadFromFile("ripper.png") )
    {
        std::cout << "Error loading ripper.png\n";
        return false;
    }

    //load ripper sprite
    for(int i = 0; i < 2; i++)
    {
        sf::IntRect newrect;
        newrect.left = i*CHUNK_SIZE*2;
        newrect.top = 0;
        newrect.width = CHUNK_SIZE*2;
        newrect.height = CHUNK_SIZE;

        sf::Sprite *newsprite = new sf::Sprite(m_RipperTXT, newrect);
        m_RipperSPR.push_back(newsprite);
    }

    std::cout << "Enemies loaded successfuly.\n";

    return true;
}

bool Engine::initPlayer()
{
    //samus sprite dimensions
    int samusheight = 40;
    int samuswidth = 28;

    std::cout << "Initializing player...\n";
    //load samus texture
    if(!m_SamusTXT.loadFromFile("samus.png"))
    {
        std::cout << "Error loading samus.png!\n";
        return false;
    }

    //create samus sprites from texture
    for(int i = 0; i < m_SamusTXT.getSize().y/samusheight; i++)
    {
        for(int n = 0; n < m_SamusTXT.getSize().x/samuswidth; n++)
        {
            sf::IntRect srect;
            srect.left = n * samuswidth;
            srect.top = i * samusheight;
            srect.width = samuswidth;
            srect.height = samusheight;
            sf::Sprite *newsprite = new sf::Sprite(m_SamusTXT, srect);
            m_SamusSPR.push_back(newsprite);
        }
    }

    //create player object
    m_Player = new Player();

    m_Player->setPositionGrid(8,10);
    std::cout << "Player initialized successfuly.\n";

    return true;
}

void Engine::mainLoop()
{
    //debug
    Enemy *newenemy = new Zoomer();
    newenemy->setPositionGrid(1,1);
    m_Enemies.push_back(newenemy);
    newenemy = new Ripper();
    newenemy->setPositionGrid(4,4);
    m_Enemies.push_back(newenemy);

    bool quit = false;

    //create viewport
    sf::View view;
    sf::Vector2f viewcenter(16*CHUNK_SIZE*CHUNK_SCALE, 15*CHUNK_SIZE*CHUNK_SCALE);
    view.setSize(CHUNK_SIZE*CHUNK_SCALE*SCREEN_WIDTH_CHUNKS, CHUNK_SIZE*CHUNK_SCALE*SCREEN_HEIGHT_CHUNKS);

    GUIobj *guiselector = NULL;
    int spritepaintertest = 2;
    bool spritepaintersupertile = true;
    bool spritepaintersupertilesnap = true;



    while(!quit)
    {
        view.setCenter(viewcenter);
        m_Screen->setView(view);

        m_Screen->clear();

        //clear debug grid boxes
        dbg_gridboxes.clear();

        sf::Event event;
        sf::Vector2i mousePosi = sf::Mouse::getPosition(*m_Screen);
        sf::Vector2f mousePos = sf::Vector2f(mousePosi);
        sf::Vector2f mousePosRel = m_Screen->mapPixelToCoords(mousePosi);




        //if in edit mode and mouse is held down, and no gui object is being selected
        if(m_Mode == MODE_EDIT_PAINT)
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) )
            {
                sf::Vector2i mcoord = screenToMapCoords(m_Screen->mapPixelToCoords(mousePosi));

                //if grid snapping on for super tiles, fit to grid pos
                if(spritepaintersupertile && spritepaintersupertilesnap)
                {
                    if(mcoord.x%2) mcoord.x--;
                    if(mcoord.y%2) mcoord.y--;
                }

                if(!m_Map->setTileAt(mcoord.x, mcoord.y, spritepaintertest))
                {
                    std::cout << "Attempting to resize map dimensions...\n";
                    m_Map->resizeToContainCoord(mcoord.x, mcoord.y);
                    m_Map->setTileAt(mcoord.x, mcoord.y, spritepaintertest);
                }

                if(spritepaintersupertile)
                {
                    //if unable to set tile at coordinate, resize map to contain coordinate
                    if(!m_Map->setTileAt(mcoord.x+1, mcoord.y+1, spritepaintertest+m_TilesDim.x+1))
                    {
                        std::cout << "Attempting to resize map dimensions...\n";
                        m_Map->resizeToContainCoord(mcoord.x+1, mcoord.y+1);
                        m_Map->setTileAt(mcoord.x+1, mcoord.y+1, spritepaintertest+m_TilesDim.x+1);
                    }
                    if(spritepaintertest == 0)
                    {
                        m_Map->setTileAt(mcoord.x, mcoord.y, 0);
                        m_Map->setTileAt(mcoord.x+1, mcoord.y, 0);
                        m_Map->setTileAt(mcoord.x, mcoord.y+1, 0);
                        m_Map->setTileAt(mcoord.x+1, mcoord.y+1, 0);
                    }
                    else
                    {
                        m_Map->setTileAt(mcoord.x+1, mcoord.y, spritepaintertest+1);
                        m_Map->setTileAt(mcoord.x, mcoord.y+1, spritepaintertest+m_TilesDim.x);
                    }


                }
            }//end mouse button pressed
            //else mouse button is not pressed, change paint mode back to edit mode
            else m_Mode = MODE_EDIT;


        }
        //update gui objects being manipulated
        else if(guiselector != NULL)
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

        //check for keys held down
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            if(m_Mode == MODE_PLAY) m_Player->runRight(true);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            if(m_Mode == MODE_PLAY) m_Player->runLeft(true);
        }
        //debug - temporary, need to clean up key held down check and key release for up/down
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            //if(m_Mode == MODE_PLAY) m_Player->setVelocityY(-PLAYER_MOVE_SPEED);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            //if(m_Mode == MODE_PLAY) m_Player->setVelocityY(PLAYER_MOVE_SPEED);
        }

        while(m_Screen->pollEvent(event) )
        {
            if(event.type == sf::Event::Closed) quit = true;
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape) quit = true;
                //move viewport around
                else if(event.key.code == sf::Keyboard::W)
                {
                    if(m_Mode == MODE_EDIT || m_Mode == MODE_EDIT_PAINT) viewcenter.y -= CHUNK_SIZE*CHUNK_SCALE;
                    else
                    {

                    }
                }
                else if(event.key.code == sf::Keyboard::S)
                {
                    if(m_Mode == MODE_EDIT || m_Mode == MODE_EDIT_PAINT) viewcenter.y += CHUNK_SIZE*CHUNK_SCALE;
                    else
                    {

                    }
                }
                else if(event.key.code == sf::Keyboard::A)
                {
                    if(m_Mode == MODE_EDIT || m_Mode == MODE_EDIT_PAINT) viewcenter.x -= CHUNK_SIZE*CHUNK_SCALE;
                }
                else if(event.key.code == sf::Keyboard::D)
                {
                    if(m_Mode == MODE_EDIT || m_Mode == MODE_EDIT_PAINT) viewcenter.x += CHUNK_SIZE*CHUNK_SCALE;
                }
                else if(event.key.code == sf::Keyboard::Space)
                {
                    if(m_Mode == MODE_PLAY)
                    {

                        if(m_Player->touchingBottom() )
                        {
                            //std::cout << "jumping with player on ground\n";
                            m_Player->setVelocityY(-PLAYER_JUMP_VEL);
                        }
                        //else std::cout << "jumping with player not on ground\n";
                    }
                }
                else if(event.key.code == sf::Keyboard::F1)
                {
                    if(m_Mode == MODE_PLAY) m_Mode = MODE_EDIT;
                    else if(m_Mode == MODE_EDIT) m_Mode = MODE_PLAY;
                    m_GUIobjs[0]->setVisible( !m_GUIobjs[0]->visible());
                }
                else if(event.key.code == sf::Keyboard::F5) m_Map->saveMapFile("map.dat");
                else if(event.key.code == sf::Keyboard::Space)
                {
                    if(m_Mode == MODE_EDIT)
                    {
                        spritepaintersupertile = !spritepaintersupertile;
                    }
                }
            }
            else if(event.type == sf::Event::KeyReleased)
            {
                if(m_Mode == MODE_PLAY)
                {
                    if(event.key.code == sf::Keyboard::D)
                    {
                        if(m_Mode == MODE_PLAY) m_Player->runRight(false);
                    }
                    else if(event.key.code == sf::Keyboard::A)
                    {
                        if(m_Mode == MODE_PLAY) m_Player->runLeft(false);
                    }
                    else if(event.key.code == sf::Keyboard::W)
                    {
                        if(m_Mode == MODE_PLAY) m_Player->setVelocityY(0);
                    }
                    else if(event.key.code == sf::Keyboard::S)
                    {
                        if(m_Mode == MODE_PLAY) m_Player->setVelocityY(0);
                    }
                }
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    //debug
                    sf::Vector2i mcoord = screenToMapCoords(m_Screen->mapPixelToCoords(mousePosi));
                    std::cout << "Mouse clicked on coord : " << mcoord.x << "," << mcoord.y << std::endl;

                    //if in editor mode...
                    if(m_Mode == MODE_EDIT)
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

                        //no gui was selected?  set painter mode on
                        if(guiselector == NULL) m_Mode = MODE_EDIT_PAINT;
                    }
                    //RETURN TO MAKE GUI MANIPULATION DEAD CODE FOR NOW
                    else continue;


                }
            }//end mouse button event
            else if(event.type == sf::Event::MouseWheelMoved)
            {
                if(event.mouseWheel.delta > 0)
                {
                    if(m_Mode == MODE_EDIT)
                    {
                        if(spritepaintersupertile && spritepaintersupertilesnap)
                        {
                            spritepaintertest +=2;
                            if(spritepaintertest%2)
                            {
                                spritepaintertest--;
                            }
                        }
                        else spritepaintertest++;
                        if(spritepaintertest >= int(m_TilesSPR.size()) )
                        {
                            spritepaintertest = 0;
                        }
                    }
                }
                if(event.mouseWheel.delta < 0)
                {
                    if(m_Mode == MODE_EDIT)
                    {
                        if(spritepaintersupertile && spritepaintersupertilesnap)
                        {
                            spritepaintertest -=2;
                            if(spritepaintertest%2)
                            {
                                spritepaintertest--;
                            }
                            if(spritepaintertest == -2) spritepaintertest = 0;
                        }
                        else spritepaintertest--;
                        if(spritepaintertest < 0 )
                        {
                            spritepaintertest = int(m_TilesSPR.size())-1;
                        }
                    }
                }

                //debug
                std::cout << "spritepainter index = " << spritepaintertest << std::endl;
            }
        }//end event handling

        //update
        m_Player->update();
        updateEnemies();


        //draw
        drawMap();
        m_Player->draw(m_Screen);
        drawEnemies();

        //draw debug grid boxes
        for(int i = 0; i < dbg_gridboxes.size(); i++)
        {
            dbg_drawGridBox(dbg_gridboxes[i].x, dbg_gridboxes[i].y);
        }

        //draw mouse test painter
        if(spritepaintertest >= 0 && spritepaintertest < int(m_TilesSPR.size()) && m_Mode == MODE_EDIT)
        {
            sf::Vector2i mtocoords = sf::Vector2i(screenToMapCoords(m_Screen->mapPixelToCoords(mousePosi)) );

            if(spritepaintersupertilesnap && spritepaintersupertile)
            {
                if(mtocoords.x%2) mtocoords.x--;
                if(mtocoords.y%2) mtocoords.y--;
            }
            //get map coord from mouse pos to snap to drawing pos
            if(spritepaintersupertile) drawSuperTile(m_Screen, mtocoords.x, mtocoords.y, spritepaintertest);
            else drawTile(m_Screen, mtocoords.x, mtocoords.y, spritepaintertest);
            //draw tile brush as mouse cursor
            //drawTileScreenCoord(m_Screen, mousePos, spritepaintertest);
        }

        //draw ui
        m_Screen->setView(m_Screen->getDefaultView());

        updateAndDrawGUIobjs(&m_GUIobjs);

        //debug info
        sf::Text debuginfo("", m_Font,12);
        debuginfo.setColor(sf::Color::Red);
        std::stringstream dinfoss;
        sf::Vector2i mcoord = screenToMapCoords(m_Screen->mapPixelToCoords(sf::Vector2i(mousePosRel)) );
        dinfoss << "mouse:" << mousePos.x << "," << mousePos.y << "  rel(" << mousePosRel.x << "," << mousePosRel.y << ")  grid(" << mcoord.x << "," << mcoord.y << ")";
        debuginfo.setString(dinfoss.str());
        m_Screen->draw(debuginfo);

        m_Screen->display();
    }
}

void Engine::updateEnemies()
{
    for(int i = 0; i < int(m_Enemies.size()); i++)
    {
        m_Enemies[i]->update();
    }
}

void Engine::updateAndDrawGUIobjs(std::vector<GUIobj*> *objlist)
{
    if(objlist == NULL) return;
    if(objlist->empty()) return;

    for(int i = 0; i < int(objlist->size()); i++)
    {
        if( !(*objlist)[i]->visible() ) continue;
        (*objlist)[i]->update(sf::Vector2f(sf::Mouse::getPosition(*m_Screen)));
        (*objlist)[i]->draw(m_Screen);

        //recursively process children
        updateAndDrawGUIobjs( (*objlist)[i]->getChildren());
    }


}

void Engine::drawTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex)
{
    drawTileScreenCoord(tscreen, sf::Vector2f(x*CHUNK_SIZE*CHUNK_SCALE, y*CHUNK_SIZE*CHUNK_SCALE), tindex);
}

void Engine::drawTileScreenCoord(sf::RenderTarget *tscreen, sf::Vector2f screencoords, unsigned int tindex)
{
    if(tindex == 0) return;

    if(tindex >= unsigned(m_TilesSPR.size()) )
    {
        std::cout << "drawTileScreenCoord : error, tile index out of bounds.  index=" << tindex << std::endl;
        return;
    }

    m_TilesSPR[tindex]->setPosition(screencoords);
    tscreen->draw(*m_TilesSPR[tindex]);
}

void Engine::drawSuperTile(sf::RenderTarget *tscreen, int x, int y, unsigned int tindex)
{
    //if trying to draw empty tile
    if(int(tindex) == 0)
    {
        //top left sprite
        drawTile(tscreen, x, y, 0);
        //top right sprite
        drawTile(tscreen, x+1, y, 0);
        //bottom left sprite
        drawTile(tscreen, x, y+1, 0);
        //bottom right sprite
        drawTile(tscreen, x+1, y+1,0);

        return;
    }

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

void Engine::drawEnemies()
{
    for(int i = 0; i < int(m_Enemies.size()); i++)
    {
        m_Enemies[i]->draw(m_Screen);
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

sf::Vector2i Engine::screenToMapCoords(sf::Vector2f mousepos)
{
    sf::Vector2i mapcoord;
    mapcoord.x = mousepos.x / (CHUNK_SIZE*CHUNK_SCALE);
    mapcoord.y = mousepos.y / (CHUNK_SIZE*CHUNK_SCALE);

    return mapcoord;
}

//debug
void Engine::dbg_addGridBox(int x, int y)
{
    dbg_gridboxes.push_back(sf::Vector2f(x, y));
}
void Engine::dbg_drawGridBox(int x, int y, sf::Color ncolor)
{
    sf::RectangleShape gbox(sf::Vector2f(CHUNK_SIZE*CHUNK_SCALE, CHUNK_SIZE*CHUNK_SCALE) );

    gbox.setFillColor(ncolor);

    gbox.setPosition(x*CHUNK_SIZE*CHUNK_SCALE, y*CHUNK_SIZE*CHUNK_SCALE);

    m_Screen->draw(gbox);
}
