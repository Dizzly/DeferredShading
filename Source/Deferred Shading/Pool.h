#ifndef POOL_H_INCLUDED
#define POOL_H_INCLUDED

//singletons stuff
//---
#include "NonCopyable.h"
//--

//data structures
//---
#include <string>
//--

template <class T>
class Pool :public NonCopyable
{
public:
	virtual bool LoadPoolObject(const std::string& file, const std::string& name)=0{return false;}
	virtual T* GetPoolObject(const std::string& name)=0{return NULL;};
};
#endif