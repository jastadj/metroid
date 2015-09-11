#include "guiobj.hpp"

GUIobj::GUIobj()
{

}

GUIobj::~GUIobj()
{

}

//////////////////////////////////////////////////////
// window pane

WindowPane::WindowPane()
{
    panebox = sf::RectangleShape(sf::Vector2f(100,100));
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
