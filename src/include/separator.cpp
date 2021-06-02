#include "separator.h"
//consider: Add error throw system.

namespace Fourest
{
    separator::separator(const std::string &_inputWords,bool _ifWhiteList):whiteList_mode(_ifWhiteList)
    {
        if(_ifWhiteList) permit(_inputWords+" ");
        else ban(_inputWords+" ");
    }

    void separator::permit(const str &_str)
    {
        for(char i : _str)
        {
            whiteList.insert(i);
            blackList.erase(i);
        }
        process();
    }

    void separator::ban(const std::string &_banWords)
    {
        for(char _banWord : _banWords)
        {
            blackList.insert(_banWord);
            whiteList.erase(_banWord);
        }
        process();
    }

    separator::~separator()
    {
        whiteList.clear();
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

    bool separator::changeMode()
    {
        whiteList_mode=!whiteList_mode;
        return whiteList_mode;
    }

    str separator::toString(charType _type)
    {
        if(_type==ALPHABET) return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        if(_type==NUMBER) return "0123456789.";
        if(_type==COMPARISON) return "<>=!";
        if(_type==CALCULATOR) return "+-*/\\%^";
        if(_type==BRACKETS) return "()<>'\"[]{}";
        if(_type==LOGIC) return "!&|^";
        if(_type==OTHER) return "@#$,?`~ ";
        return "";
    }

    void separator::ban(charType _type)
    {
        ban(toString(_type));
    }

    void separator::permit(charType _type)
    {
        permit(toString(_type));
    }

    void separator::clear()
    {
        whiteList.clear();
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
            if(whiteList_mode) legal=whiteList.find(source[i])!=whiteList.end();
            else legal=blackList.find(source[i])==blackList.end();

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

    std::vector<str> separator::restWords() const
    {
        return wordsBeBaned;
    }

    std::vector<double> separator::getNumber()
    {
        separator temp(toString(NUMBER),true),onlyInt(".");
        temp.process(source);

        std::vector<double> output;
        for(int k=0;k<temp.size();++k)
        {
            onlyInt.process(temp[k]);
            if(onlyInt.size()==1) output.push_back(toInt(onlyInt[0]));
            else output.push_back(toInt(onlyInt[0])+toInt(onlyInt[1])/double(onlyInt[1].size()));
        }
        return output;
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

    bool checkLength(const separator &_sep,int _len)
    {
        if(_sep.size()!=_len) return false;
        return true;
    }


}















