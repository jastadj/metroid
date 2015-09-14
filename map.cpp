#include "map.hpp"
#include <fstream>
#include <iostream>
#include "tools.hpp"

Map::Map()
{


}

Map::~Map()
{

}

bool Map::loadMapFile(std::string filename)
{
    std::ifstream ifile;
    ifile.open(filename.c_str());

    if(!ifile.is_open())
    {
        std::cout << "Map::loadMapFile error, unable to open map file :" << filename << std::endl;
        return false;
    }

    //vars to store expected map data width
    int datawidth = -1;
    //map line iterator pos
    int it = 0;

    while(!ifile.eof())
    {
        //buffer for file line
        std::string buf;

        std::getline(ifile, buf);

        if(buf == "\n" || buf == "") continue;

        std::vector< std::string > linedata = csvParse(buf);

        //if first line being read, set datawidth
        if(datawidth == -1) datawidth = int(linedata.size());
        //this map line data does not match expected
        else if( int(linedata.size()) != datawidth)
        {
            std::cout << "Error loading map - map line " << it << " does not match expected width of " << datawidth <<
                ". Map data corrupt." << std::endl;
            ifile.close();
            return false;
        }

        //process parsed strings as data
        m_MapData.resize( m_MapData.size() + 1);
        for(int i = 0; i < int(linedata.size()); i++)
        {
            m_MapData[it].push_back( atoi(linedata[i].c_str() ) );
        }


        it++;
    }

    m_MapDim.x = datawidth;
    m_MapDim.y = int(m_MapData.size());

    std::cout << "Loaded map with dimensions : " << m_MapDim.x << "," << m_MapDim.y << std::endl;

    ifile.close();
    return true;
}

const int Map::getTileAt(int x, int y)
{
    if(x < 0 || y < 0 || x >= int(m_MapDim.x) || y >= int(m_MapDim.y) )
    {
        std::cout << "Error getting tile at " << x << "," << y << ".  Out of map data bounds!  Returning -1 tile\n";
        return -1;
    }

    else return m_MapData[y][x];
}

bool Map::setTileAt(int x, int y, int tileindex)
{
    if(x < 0 || y < 0 || x >= int(m_MapDim.x) || y >= int(m_MapDim.y) )
    {
        std::cout << "Map setTileAt error : coords " << x << "," << y << " are out of map bounds ("
        << m_MapDim.x << "," << m_MapDim.y << ")\n";
        return false;
    }

    m_MapData[y][x] = tileindex;

    return true;
}

bool Map::resizeToContainCoord(int x, int y)
{
    std::cout << "Resizing map to contain coord " << x << "," << y << "...\n";

    //dont handle negative number shift for now....
    if(x < 0 || y < 0) return false;

    //resize width
    if( unsigned(x) >= m_MapDim.x)
    {
        unsigned int oldwidth = m_MapDim.x;
        m_MapDim.x = unsigned(x)+1;
        //resize array
        for(int i = 0; i < int(m_MapData.size()); i++) m_MapData[i].resize(x+1);

        for(int i = 0; i < int(m_MapData.size()); i++)
            for(int n = 0; n < int(m_MapData[i].size()); n++)
        {
            if( n >= oldwidth) m_MapData[i][n] = -1;
        }
    }

    //resize height
    if( unsigned(y) > m_MapDim.y)
    {
        unsigned int oldheight = m_MapDim.y;

        m_MapDim.y = unsigned(y)+1;

        //resize array height
        m_MapData.resize(m_MapDim.y);
        //resize array width for new height lines
        for(int i = 0; i < int(m_MapData.size()); i++)
        {
            m_MapData[i].resize(m_MapDim.x);

            if(i >= oldheight) for(int n = 0; n < int(m_MapData[i].size()); n++) m_MapData[i][n] = -1;
        }

    }

    std::cout << "Map dimensions are " << m_MapDim.x << "," << m_MapDim.y << std::endl;
    std::cout << "Map data array dimensions are " << m_MapData[0].size() << "," << m_MapData.size()-1 << std::endl;

    return true;

}
