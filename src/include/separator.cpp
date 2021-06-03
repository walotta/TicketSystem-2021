#include "separator.h"


namespace Fourest
{
    separator::separator(const std::string &_inputWords)
    {
        ban(_inputWords+" ");
    }

    void separator::ban(const std::string &_banWords)
    {
        for(char _banWord : _banWords) blackList.insert(_banWord);
        process();
    }

    separator::~separator()
    {
        blackList.clear();
        words.clear();
    }

    str separator::operator[](int _pos) const
    {
        if(_pos<0 || _pos>=words.size()) throw "ERROR";
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
        std::string input;
        getline(_is,input);
        process(input);
        if(_is.eof()) return false;
        return true;
    }

    std::vector<str> separator::content() const
    {
        return words;
    }


    void separator::clear()
    {
        blackList.clear();
        words.clear();
    }

    void separator::process()
    {
        words.clear();
        int startPos=0,Length=0; // For legal chars.
        int _startPos=0,Len=0; // For illegal chars.
        for(int i=0;i<source.size();++i)
        {
            bool legal=false;
            legal=blackList.find(source[i])==blackList.end();

            if(!legal)
            {
                if(Length>0) words.push_back(source.substr(startPos,Length));
                Length=0;
                if(Len==0) _startPos=i;
                ++Len;
            }
            else
            {
                if(Length==0) startPos=i;
                ++Length;
                if(Len>0) wordsBeBaned.push_back(source.substr(_startPos,Len));
                Len=0;
            }
        }
        if(Length>0) words.push_back(source.substr(startPos,Length));
    }

    int separator::toInt(const str &_number)
    {
        int output=0;
        for(char i : _number)
        {
            if(i>'9' || i<'0') throw "ERROR";
            output=10*output+int(i)-int('0');
        }
        return output;
    }

    double toNumber(const str &_str)
    {
        //consider: To upgrade this function.
        double output=0;
        int dotPos=-1;
        for(int i=0;i<_str.size();++i)
        {
            if(_str[i]=='.') {dotPos=i; break;}
            if(_str[i]>'9' || _str[i]<'0') throw "ERROR";
            output=10*output+int(_str[i])-int('0');
        }

        double latterPart=0;
        if(dotPos!=-1)
            for(int i=_str.size()-1;i>dotPos;--i)
            {
                if(_str[i]>'9' || _str[i]<'0') throw "ERROR";
                latterPart=latterPart/10+(int(_str[i])-int('0'))/10.0;
            }
        return output+latterPart;
    }
}















