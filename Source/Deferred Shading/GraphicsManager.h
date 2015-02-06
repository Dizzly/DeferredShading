#ifndef GRAPHICS_MANAGER_H_INCLUDED
#define GRAPHICS_MANAGER_H_INCLUDED

//Singleton prep
//---
#include "NonCopyable.h"
//---


class State;


class GraphicsManager: public NonCopyable
{
public:
	virtual ~GraphicsManager()=0{};
	virtual bool Init()=0{};
	virtual bool ReInit()=0{};
	virtual bool Destroy()=0{};
	virtual void BeginDraw()=0{};
	virtual void EndDraw()=0{};
	virtual void ManageState(State*){}
};

#endif