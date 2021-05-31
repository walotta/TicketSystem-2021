
#ifndef FOUREST_SEPARATOR_H
#define FOUREST_SEPARATOR_H


#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

namespace Fourest
{
    typedef std::string str;
    enum charType{ALPHABET,NUMBER,COMPARISON,CALCULATOR,BRACKETS,LOGIC,OTHER};

    class separator
    {
        std::string source;
        std::vector<str> words;
        std::vector<str> wordsBeBaned;
        std::unordered_set<char> blackList;
        std::unordered_set<char> whiteList;
        bool whiteList_mode=false;

        static str toString(charType);
        void process();
        static int toInt(const str &);

    public:
        separator()=default;
        explicit separator(const str &,bool _ifWhiteList=false);
        ~separator();

        int size() const;
        bool changeMode();
        void ban(charType);
        void ban(const str &);
        void clear();
        void permit(charType);
        void permit(const str &);
        void process(const str &);
        bool readLine(std::istream &);
        std::vector<str> content() const;
        std::vector<str> restWords() const;
        std::vector<double> getNumber();

        str operator[](int) const;

    };

    double toNumber(const str &);
    bool checkLength(const separator &,int);

}

#endif //FOUREST_SEPARATOR_H
