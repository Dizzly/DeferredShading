#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

//for purposes of singleton
//---
#include "NonCopyable.h"
//--

//pure virtual timer class
//for specific implementation
class Timer
{
public:
	virtual bool Init()=0{}
	virtual float TotalTime()const=0 {}
	virtual float DeltaTime()const=0{}

	virtual void Tick()=0{};

	virtual void Reset()=0{};
	virtual void Start()=0{};
	virtual void Stop()=0{};
};


#endif