#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED
//Singleton prep
//---
#include "NonCopyable.h"
//all states will be singletons, this is a preinclude for childclasses
#include "Singleton.h"
//---
class State
{
public:
	virtual ~State()=0{};
	virtual void Draw()=0{};
	virtual void Update()=0{};
	virtual void OnActive()=0{};
	virtual void OnInactive()=0{};
};



#endif