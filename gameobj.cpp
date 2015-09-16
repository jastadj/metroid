#include "gameobj.hpp"

GameOBJ::GameOBJ()
{
    m_Scale = sf::Vector2f(CHUNK_SCALE, CHUNK_SCALE);
    m_Rotation = 0;
}

GameOBJ::~GameOBJ()
{

}

//void GameOBJ::update(){}
//void GameOBJ::draw(sf::RenderTarget *trender) {}
