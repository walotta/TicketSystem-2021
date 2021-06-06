
#ifndef FOUREST_SEPARATOR_H
#define FOUREST_SEPARATOR_H


#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

namespace Fourest
{
    typedef std::string str;

    class separator
    {
        std::string source;
        std::vector<str> words;
        std::vector<str> wordsBeBaned;
        std::unordered_set<char> blackList;

        void process();
        static int toInt(const str &);

    public:
        separator()=default;
        explicit separator(const str &);
        ~separator();

        int size() const;
        void ban(const str &);
        void clear();
        void process(const str &);
        bool readLine(std::istream &);
        std::vector<str> &content();

        str &operator[](int);
        bool empty() const {return words.empty();}

    };

    double toNumber(const str &);
}

#endif //FOUREST_SEPARATOR_H
