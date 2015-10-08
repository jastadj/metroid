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
    //initPlayer loads in the player graphics, creates additional mirrored graphics, and stores this into
    //a template.  this template is then used for color masking to create images of samus variants to store
    //in memory

    //samus sprite dimensions
    int samusheight = 40;
    int samuswidth = 28;
    //color mask for color replacement to mask each samus color variant
    sf::Color colormask(255,0,255,255);

    //load master sprite sheet image
    sf::Image samusmasterimg;
    //create samus image template, for creating color variants
    std::vector<sf::Image> m_SamusIMG_template;
    int variant_index = 0;

    std::cout << "Initializing player...\n";


    //load samums image sprite sheet
    if(!samusmasterimg.loadFromFile("samus.png"))
    {
        std::cout << "Error loading samus.png!\n";
        return false;
    }

    //break apart samus image sprite sheet for each frame and load into samus color template
    for(int i = 0; i < PLAYER_FRAMES_IN_SET; i++)
    {
        sf::IntRect framerect;
        framerect.left = i*samuswidth;
        framerect.top = 0*samusheight;
        framerect.width = samuswidth;
        framerect.height = samusheight;

        //push new image onto samus image list
        m_SamusIMG_template.resize( m_SamusIMG_template.size()+1);
        m_SamusIMG_template.back().create(samuswidth, samusheight, sf::Color::Transparent);
        m_SamusIMG_template.back().copy(samusmasterimg, 0, 0, framerect, true);

    }
    //copy each image from samus left facing images and flip horizontally and add
    for(int i = 0; i < PLAYER_FRAMES_IN_SET; i++)
    {
        //push new image onto samus image list
        m_SamusIMG_template.resize( m_SamusIMG_template.size()+1);
        m_SamusIMG_template.back() = m_SamusIMG_template[i];//.copy(m_SamusIMG[i], 0, 0, sf::IntRect(0,0,0,0), true);
        //flip
        m_SamusIMG_template.back().flipHorizontally();
    }

    //samus green variant
    m_SamusIMG.push_back(m_SamusIMG_template);
    for(int i = 0; i < int(m_SamusIMG.back().size()); i++)
    {
        for(int ny = 0; ny < samusheight; ny++)
        {
            for(int nx = 0; nx < samuswidth; nx++)
            {
                if(m_SamusIMG.back()[i].getPixel(nx,ny) == colormask)
                {
                    m_SamusIMG.back()[i].setPixel(nx,ny, sf::Color(0,0x94,0,255) );
                }
            }
        }
    }

    //samus cyan variant 00 e8 d8
    m_SamusIMG.push_back(m_SamusIMG_template);
    for(int i = 0; i < int(m_SamusIMG.back().size()); i++)
    {
        for(int ny = 0; ny < samusheight; ny++)
        {
            for(int nx = 0; nx < samuswidth; nx++)
            {
                if(m_SamusIMG.back()[i].getPixel(nx,ny) == colormask)
                {
                    m_SamusIMG.back()[i].setPixel(nx,ny, sf::Color(0x00, 0xe8, 0xd8, 255) );
                }
            }
        }
    }

    //create texture for each samus image / variant
    for(int n = 0; n < int(m_SamusIMG.size()); n++)
    {
        //resize texture array
        m_SamusTXT.resize( m_SamusTXT.size()+1);

        //push new textures into variant array
        for(int i = 0; i < int(m_SamusIMG[n].size()); i++)
        {
            m_SamusTXT.back().resize(m_SamusTXT.back().size()+1);
            m_SamusTXT.back()[i].loadFromImage(m_SamusIMG[n][i]);

        }
    }


    //create sprite from each samus texture
    for(int n = 0; n < int(m_SamusTXT.size()); n++)
    {
        m_SamusSPR.resize(m_SamusSPR.size()+1);

        for(int i = 0; i < int(m_SamusTXT[n].size()); i++)
        {
            sf::Sprite *newsprite = new sf::Sprite(m_SamusTXT[n][i]);
            m_SamusSPR[n].push_back(newsprite);
        }
    }

    //load bullet texture
    if(!m_BulletTXT.loadFromFile("bullet.png"))
    {
        std::cout << "Error loading bullet.png\n";
        return false;
    }

    //create bullet sprites
    for(int i = 0; i < m_BulletTXT.getSize().y / CHUNK_SIZE; i++)
    {
        for(int n = 0; n < m_BulletTXT.getSize().x / CHUNK_SIZE; n++)
        {
            sf::IntRect myrect;
            myrect.left = n * CHUNK_SIZE;
            myrect.top = i * CHUNK_SIZE;
            myrect.width = CHUNK_SIZE;
            myrect.height = CHUNK_SIZE;

            sf::Sprite *newsprite = new sf::Sprite(m_BulletTXT, myrect);
            m_BulletSPR.push_back(newsprite);
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
                    //std::cout << "Mouse clicked on coord : " << mcoord.x << "," << mcoord.y << std::endl;

                    //if in editor mode...
                    if(m_Mode == MODE_EDIT)
                    {
                        /*
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
                        */

                        getGUIclickedOn(&m_GUIobjs, mousePos, guiselector);


                        if(guiselector != NULL)
                        {
                            //guiselector = m_GUIobjs[i];

                            guiselector->m_ClickedOffset = mousePos - sf::Vector2f(guiselector->getRect().left,
                                                                   guiselector->getRect().top);
                        }
                        //no gui was selected?  set painter mode on
                        else m_Mode = MODE_EDIT_PAINT;
                    }
                    else if(m_Mode == MODE_PLAY)
                    {
                        //shoot bullets and shit
                        m_Bullets.push_back( m_Player->fireBullet());
                    }
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
        for(int i = int(m_Bullets.size()-1); i >= 0; i--)
        {
            m_Bullets[i]->update();

            if(m_Bullets[i]->isDead())
            {
                delete m_Bullets[i];
                m_Bullets.erase( m_Bullets.begin() + i);
            }
        }


        //draw
        drawMap();
        m_Player->draw(m_Screen);
        drawEnemies();
        for(int i = 0; i < int(m_Bullets.size()); i++) m_Bullets[i]->draw(m_Screen);

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

void Engine::getGUIclickedOn(std::vector<GUIobj*> *objlist, sf::Vector2f mousePos, GUIobj *selectedGUI)
{

    //need to figure out how to reach down recursively and return gui obj clicked on if valid

    //check for gui objs if mouse clicked on
    for(int i = 0; i < int(objlist->size()); i++)
    {
        if(!(*objlist)[i]->visible()) continue;
        if( (*objlist)[i]->mouseOver(mousePos))
        {
            selectedGUI = (*objlist)[i];

            selectedGUI->m_ClickedOffset = mousePos - sf::Vector2f(selectedGUI->getRect().left,
                                                   selectedGUI->getRect().top);
            break;
        }
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

std::vector< sf::Sprite* > *Engine::getSamusSPR(int colorvariant)
{
    if(colorvariant < 0 || colorvariant >= int(m_SamusSPR.size()) )
    {
       //color variant out of bounds of sprite list!
        std::cout << "Error getting samus sprite (color variant out of bounds!\n";
        colorvariant = 0;
    }

    return &m_SamusSPR[colorvariant];
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
