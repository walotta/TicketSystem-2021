
#ifndef FOUREST_SEPARATOR_H
#define FOUREST_SEPARATOR_H


#include <iostream>
#include <string>
#include "STL.h"

namespace Fourest
{
    typedef std::string str;

    class separator
    {
        std::string source;
        vector<str> words;
        char ban_word;

        void process();
    public:
        separator()=default;
        explicit separator(const char &y):ban_word(y){}
        ~separator();

        int size() const;
        void clear();
        void process(const str &);
        bool readLine(std::istream &);
        vector<str> &content();

        str &operator[](int);
        bool empty() const {return words.empty();}
    };
}

#endif //FOUREST_SEPARATOR_H
