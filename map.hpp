#ifndef CLASS_MAP
#define CLASS_MAP

#include <string>
#include <vector>
#include <SFML\Graphics.hpp>

class Map
{
private:

    std::string m_Filename;

    sf::Vector2u m_MapDim;

    std::vector< std::vector<int> > m_MapData;

public:
    Map();
    ~Map();

    bool loadMapFile(std::string filename);
    bool saveMapFile(std::string filename);
    const sf::Vector2u getMapDims() const {return m_MapDim;}
    const int getTileAt(int x, int y);
    bool setTileAt(int x, int y, int tileindex);

    bool resizeToContainCoord(int x, int y);

    int getMapWidth() { return m_MapDim.x;}
    int getMapHeight() { return m_MapDim.y;}

    const std::vector< std::vector<int> > *getMapData() const {return &m_MapData;}
};
#endif // CLASS_MAP
