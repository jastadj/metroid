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

void WindowPane::update()
{
    panebox.setPosition(m_Pos);
}
