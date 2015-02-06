#ifndef INPUT_MANAGER_H_INCLUDED
#define INPUT_MANAGER_H_INCLUDED




//Singleton prep
//---
#include "NonCopyable.h"
//---


class InputManager
{
public:
	virtual ~InputManager()=0{};
	virtual bool Init()=0{};
	virtual bool Destroy()=0{};

	virtual void Update()=0{};

	virtual bool ShouldQuit()=0{return true;};
};

#endif