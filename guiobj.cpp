#include "guiobj.hpp"
#include <iostream>

GUIobj::GUIobj()
{
    m_Visible = false;
    m_Movable = false;

}

GUIobj::~GUIobj()
{

}

//////////////////////////////////////////////////////
// window pane

WindowPane::WindowPane(sf::Vector2f panesize)
{
    panebox = sf::RectangleShape(panesize);
    panebox.setFillColor(sf::Color(0,200,50,128));
    panebox.setOutlineThickness(2);
    panebox.setOutlineColor(sf::Color(0,255,0));

}

WindowPane::~WindowPane()
{

}

void WindowPane::draw(sf::RenderTarget *rtarget)
{
    rtarget->draw(panebox);
}

void WindowPane::update(sf::Vector2f mousePos)
{
    panebox.setPosition(m_Pos);
}

sf::FloatRect WindowPane::getRect()
{
    return panebox.getGlobalBounds();
}

bool WindowPane::mouseOver(sf::Vector2f mousepos)
{
    if(panebox.getGlobalBounds().contains(mousepos)) return true;
    else return false;
}

/////////////////////////////////////////////////////////////////////
//  SPRITE Button
SpriteButton::SpriteButton(sf::Sprite *tsprite)
{
    if(tsprite == NULL)
    {
        std::cout << "Error creating sprite button, target sprite is NULL!\n";
    }

    m_Sprite = tsprite;

}

SpriteButton::~SpriteButton()
{

}

void SpriteButton::draw(sf::RenderTarget *rtarget)
{
    rtarget->draw(*m_Sprite);
}

void SpriteButton::update(sf::Vector2f mousePos)
{
    m_Sprite->setPosition(m_Pos);
}

sf::FloatRect SpriteButton::getRect()
{
    return m_Sprite->getGlobalBounds();
}

bool SpriteButton::mouseOver(sf::Vector2f mousepos)
{
    if(m_Sprite->getGlobalBounds().contains(mousepos)) return true;
    else return false;
}

void SpriteButton::doClicked()
{

}
