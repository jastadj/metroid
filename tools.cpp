#include "tools.hpp"

std::vector<std::string> csvParse(std::string pstring, char delim)
{
    std::vector<std::string> parsedStrings;

    //don't use " as delimiter, bad things will happen, detect this and return a blank list
    if(delim == '"') return parsedStrings;

    //size_t lpos = 0; //last position
    //size_t qpos = 0; //quote position

    bool inquotes = false;
    std::string currentstring;

    //walk through each character in string
    for(int i = 0; i < int(pstring.length()); i++)
    {
        //if character is a the delim, stop current string, add to list, and start new string
        if(pstring[i] == delim && !inquotes)
        {
            parsedStrings.push_back(currentstring);

            currentstring = "";

            //lpos = i;
        }
        //are we at the end of the string?
        else if(i == int(pstring.length()-1))
        {
            //if last character is not quote
            if(pstring[i] != '"') currentstring.push_back( pstring[i]);
            parsedStrings.push_back(currentstring);
        }
        //if entering a cell in quotes
        else if(pstring[i] == '"')
        {
            if(!inquotes)
            {
                inquotes = true;
                if(pstring[i+1] == '"') currentstring.push_back('"');
            }
            else
            {
                //if next character is a quote (double quotes) add to string
                if(pstring[i+1] == '"') currentstring.push_back('"');
                //else if next character is the delimiter
                else if(pstring[i+1] == delim)
                {
                    inquotes = false;
                }
            }

        }
        else currentstring.push_back(pstring[i]);

    }


    return parsedStrings;

}

