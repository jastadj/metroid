#ifndef CLASS_GUIOBJ
#define CLASS_GUIOBJ

#include <SFML\Graphics.hpp>

class GUIobj
{
protected:

    sf::Vector2f m_Pos;

    bool m_Visible;
    bool m_Movable;

    GUIobj *m_Parent;
    std::vector<GUIobj*> m_Children;

public:
    GUIobj();
    virtual ~GUIobj();

    sf::Vector2f m_ClickedOffset;

    virtual void draw(sf::RenderTarget *rtarget)=0;
    virtual void update(sf::Vector2f mousePos)=0;

    const sf::Vector2f getPosition() const {return m_Pos;}
    void setPosition(sf::Vector2f newpos) { m_Pos = newpos;}
    void setPosition(int x, int y) { setPosition(sf::Vector2f(x,y));}

    virtual sf::FloatRect getRect()=0;
    virtual bool mouseOver(sf::Vector2f mousepos)=0;
    virtual void doClicked() {};

    void setVisible(bool nvis) { m_Visible = nvis;}
    void setMovable(bool nmove) { m_Movable = nmove;}
    bool visible() { return m_Visible;}
    bool movable() { return m_Movable;}

    //void setParent(GUIobj *tparent) { m_Parent = tparent;}
    GUIobj *getParent() { return m_Parent;}
    void addChild(GUIobj *tchild);
    std::vector<GUIobj*> *getChildren() { return &m_Children;}

};

class WindowPane: public GUIobj
{
private:

    sf::RectangleShape panebox;

public:
    WindowPane(sf::Vector2f panesize);
    ~WindowPane();

    void draw(sf::RenderTarget *rtarget);
    void update(sf::Vector2f mousePos);

    sf::FloatRect getRect();
    bool mouseOver(sf::Vector2f mousepos);
};

class SpriteButton: public GUIobj
{
private:

    sf::Sprite *m_Sprite;

public:
    SpriteButton(sf::Sprite *tsprite);
    ~SpriteButton();

    void draw(sf::RenderTarget *rtarget);
    void update(sf::Vector2f mousePos);

    sf::FloatRect getRect();
    bool mouseOver(sf::Vector2f mousepos);

    void doClicked();

};

#endif // CLASS_GUIOBJ
