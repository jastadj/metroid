#include "gameobj.hpp"

GameOBJ::GameOBJ()
{
    m_Scale = sf::Vector2f(CHUNK_SCALE, CHUNK_SCALE);
    m_Rotation = 0;
}

GameOBJ::~GameOBJ()
{

}

std::vector< std::vector<int> > GameOBJ::getOccupiedTiles()
{
    std::vector< std::vector<int> > otiles;

    //tile width
    //int twidth = m_Transform.transformRect()

}

//void GameOBJ::update(){}
//void GameOBJ::draw(sf::RenderTarget *trender) {}
