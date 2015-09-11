#ifndef CLASS_GUIOBJ
#define CLASS_GUIOBJ

#include <SFML\Graphics.hpp>

class GUIobj
{
protected:

    sf::Vector2f m_Pos;

public:
    GUIobj();
    virtual ~GUIobj();

    virtual void draw(sf::RenderTarget *rtarget)=0;
    virtual void update()=0;

    const sf::Vector2f getPosition() const {return m_Pos;}
    void setPosition(sf::Vector2f newpos) { m_Pos = newpos;}
    void setPosition(int x, int y) { setPosition(sf::Vector2f(x,y));}
};

class WindowPane: public GUIobj
{
private:

    sf::RectangleShape panebox;

public:
    WindowPane();
    ~WindowPane();

    void draw(sf::RenderTarget *rtarget);
    void update();
};
#endif // CLASS_GUIOBJ
