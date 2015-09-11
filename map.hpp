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
    const sf::Vector2u getMapDims() const {return m_MapDim;}
    const int getTileAt(int x, int y);
};
#endif // CLASS_MAP
