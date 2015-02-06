#ifndef SINGLETON_H
#define SINGLETON_H
#include "NonCopyable.h"

template <class T>
class Singleton : public NonCopyable
{
public:
    static T* Instance()
    {
        static T t;
        return &t;
    }
};

#endif