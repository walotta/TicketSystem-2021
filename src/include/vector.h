//
// Created by Fourest on 2021/6/5.
//

#ifndef FOUREST_VECTOR_H
#define FOUREST_VECTOR_H

namespace Fourest
{
    const int vector_initial_length=34;

    template<typename valueType>
    class vector
    {
        valueType *value;
        int length=0;
        int _size=0;

        void double_size()
        {
            valueType *temp=new valueType[2*length];
            for(int i=0;i<length;++i) temp[i]=value[i];
            delete [] value;
            value=temp;
            length=length<<1;
        }
    public:
        vector():length(vector_initial_length)
        {
            value=new valueType[length];
        }
        void push_back(const valueType &v)
        {
            if(_size+1==length) double_size();
            value[_size++]=v;
        }
        int size() const {return _size;}
        valueType &operator[](const unsigned int &pos) {return value[i];}
    };

}



#endif //FOUREST_VECTOR_H
