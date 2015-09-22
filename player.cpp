#include "player.hpp"
#include "engine.hpp"

Player::Player()
{
    //set player bounding box dimensions
    //m_BoundingBox.
}

Player::~Player()
{

}

void Player::update()
{
    updateTransform();
}

void Player::draw(sf::RenderTarget *trender)
{
    //get engine ref
    Engine *eptr = NULL;
    eptr = Engine::getInstance();

    sf::Sprite *tsprite = NULL;
    tsprite = (*eptr->getSamusSPR())[0];

    trender->draw(*tsprite, m_Transform);
}
