#include "gameobj.hpp"
#include <iostream>
#include <string>

#include "engine.hpp"

GameOBJ::GameOBJ()
{
    m_Scale = sf::Vector2f(CHUNK_SCALE, CHUNK_SCALE);
    m_Rotation = 0;
    m_Frame = 0;
}

GameOBJ::~GameOBJ()
{

}

void GameOBJ::drawBoundingBox(sf::RenderTarget *trender)
{
    sf::RectangleShape bbox( sf::Vector2f(m_BoundingBox.width-2, m_BoundingBox.height-2) );
    bbox.setOutlineThickness(1);

    bbox.setPosition(m_BoundingBox.left+1, m_BoundingBox.top+1);
    bbox.setFillColor(sf::Color::Transparent);
    bbox.setOutlineColor(sf::Color::Green);

    trender->draw(bbox);
}

bool GameOBJ::validPosition()
{
    //get engine reference
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    sf::Vector2f topleft( float(m_BoundingBox.left), float(m_BoundingBox.top));
    sf::Vector2f bottomright( topleft.x + m_BoundingBox.width, topleft.y + m_BoundingBox.height);
    sf::Vector2i p0 = eptr->screenToMapCoords(topleft);
    sf::Vector2i p1 = eptr->screenToMapCoords(bottomright);

    for(int i = p0.y; i <= p1.y; i++)
    {
        for(int n = p0.x; n <= p1.x; n++)
        {
            if(i < 0 || n < 0 || i >= eptr->getMapHeight() || n >= eptr->getMapWidth()) return false;
            else
            {
                const std::vector< std::vector<int> > *mdata = eptr->getMapData();

                if( (*mdata)[i][n] != 0) return false;
            }
        }
    }

    return true;
}

bool GameOBJ::collidingBottom()
{
    //get engine reference
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    bool foundcollision = false;

    sf::Vector2f topleft( float(m_BoundingBox.left), float(m_BoundingBox.top));
    sf::Vector2f bottomright( topleft.x + m_BoundingBox.width, topleft.y + m_BoundingBox.height);
    sf::Vector2i p0 = eptr->screenToMapCoords(topleft);
    sf::Vector2i p1 = eptr->screenToMapCoords(bottomright);

        for(int n = p0.x; n <= p1.x; n++)
        {
            if(p1.x < 0 || n < 0 || p1.y+1 >= eptr->getMapHeight() || n >= eptr->getMapWidth()) return true;
            else
            {
                const std::vector< std::vector<int> > *mdata = eptr->getMapData();

                if( (*mdata)[p1.y][n] != 0)
                {
                    //debug
                    eptr->dbg_addGridBox(n, p1.y);

                    foundcollision = true;
                }
            }
        }

    return foundcollision;
}

bool GameOBJ::collidingTop()
{
    //get engine reference
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    bool foundcollision = false;

    sf::Vector2f topleft( float(m_BoundingBox.left), float(m_BoundingBox.top));
    sf::Vector2f bottomright( topleft.x + m_BoundingBox.width, topleft.y + m_BoundingBox.height);
    sf::Vector2i p0 = eptr->screenToMapCoords(topleft);
    sf::Vector2i p1 = eptr->screenToMapCoords(bottomright);

        for(int n = p0.x; n <= p1.x; n++)
        {
            if(p1.x < 0 || n < 0 || p1.y >= eptr->getMapHeight() || n >= eptr->getMapWidth()) return true;
            else
            {
                const std::vector< std::vector<int> > *mdata = eptr->getMapData();

                if( (*mdata)[p0.y][n] != 0)
                {
                    //debug
                    eptr->dbg_addGridBox(n, p0.y);

                    foundcollision = true;
                }
            }
        }

    return foundcollision;
}

bool GameOBJ::collidingRight()
{
    //get engine reference
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    bool foundcollision = false;

    sf::Vector2f topleft( float(m_BoundingBox.left), float(m_BoundingBox.top));
    sf::Vector2f bottomright( topleft.x + m_BoundingBox.width, topleft.y + m_BoundingBox.height);
    sf::Vector2i p0 = eptr->screenToMapCoords(topleft);
    sf::Vector2i p1 = eptr->screenToMapCoords(bottomright);

        for(int n = p0.y; n <= p1.y; n++)
        {
            if(p1.x < 0 || n < 0 || p1.y >= eptr->getMapHeight() || n >= eptr->getMapWidth()) return true;
            else
            {
                const std::vector< std::vector<int> > *mdata = eptr->getMapData();

                if( (*mdata)[n][p1.x] != 0)
                {
                    //debug
                    eptr->dbg_addGridBox(p1.x, n);

                    foundcollision = true;
                }
            }
        }

    //if(foundcollision) std::cout << "touching right\n";
    return foundcollision;
}

bool GameOBJ::collidingLeft()
{
    //get engine reference
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    bool foundcollision = false;

    sf::Vector2f topleft( float(m_BoundingBox.left), float(m_BoundingBox.top));
    sf::Vector2f bottomright( topleft.x + m_BoundingBox.width, topleft.y + m_BoundingBox.height);
    sf::Vector2i p0 = eptr->screenToMapCoords(topleft);
    sf::Vector2i p1 = eptr->screenToMapCoords(bottomright);

        for(int n = p0.y; n <= p1.y; n++)
        {
            if(p1.x < 0 || n < 0 || p1.y >= eptr->getMapHeight() || n >= eptr->getMapWidth()) return true;
            else
            {
                const std::vector< std::vector<int> > *mdata = eptr->getMapData();

                if( (*mdata)[n][p0.x] != 0)
                {
                    //debug
                    eptr->dbg_addGridBox(p0.x, n);

                    foundcollision = true;
                }
            }
        }
    //if(foundcollision) std::cout << "touching left\n";
    return foundcollision;
}

bool GameOBJ::touchingBottom()
{
    bool touching = false;

    if(collidingBottom()) return false;

    m_Position.y++;
    updateTransform();

    if(collidingBottom()) touching = true;

    m_Position.y--;
    updateTransform();

    return touching;

}

bool GameOBJ::touchingTop()
{
    bool touching = false;

    if(collidingTop()) return false;

    m_Position.y--;
    updateTransform();

    if(collidingTop()) touching = true;

    m_Position.y++;
    updateTransform();

    return touching;
}

bool GameOBJ::touchingLeft()
{
    bool touching = false;

    if(collidingLeft()) return false;

    m_Position.x--;
    updateTransform();

    if(collidingLeft()) touching = true;

    m_Position.x++;
    updateTransform();

    return touching;
}

bool GameOBJ::touchingRight()
{
    bool touching = false;

    if(collidingRight()) return false;

    m_Position.x++;
    updateTransform();

    if(collidingRight()) touching = true;

    m_Position.x--;
    updateTransform();

    return touching;
}

void GameOBJ::updateTransform()
{

    m_Transform = sf::Transform();
    m_Transform.scale(m_Scale);
    m_Transform.translate(m_Position);

    sf::Vector2f testpoint = m_Transform.transformPoint(0,0);
    m_BoundingBox.left = testpoint.x + m_BoundingBoxOffset.x;
    m_BoundingBox.top = testpoint.y + m_BoundingBoxOffset.y;
}
