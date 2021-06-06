#include "separator.h"


namespace Fourest
{
    separator::~separator()
    {
        words.clear();
    }

    str &separator::operator[](int _pos)
    {
        return words[_pos];
    }

    int separator::size() const
    {
        return words.size();
    }

    void separator::process(const std::string &_str)
    {
        source=_str;
        process();
    }

    bool separator::readLine(std::istream &_is)
    {
        getline(_is,source);
        process();
        if(_is.eof()) return false;
        return true;
    }

    vector<str> &separator::content()
    {
        return words;
    }


    void separator::clear()
    {
        words.clear();
    }

    void separator::process()
    {
        words.clear();
        int startPos=0,Length=0; // For legal chars.
        int _startPos=0,Len=0; // For illegal chars.
        for(int i=0;i<source.size();++i)
        {
            if(ban_word==source[i])
            {
                if(Length>0) words.push_back(source.substr(startPos,Length));
                Length=0;
                ++Len;
            }
            else
            {
                if(Length==0) startPos=i;
                ++Length;
                Len=0;
            }
        }
        if(Length>0) words.push_back(source.substr(startPos,Length));
    }

}















