#ifndef DX11_TIMER_H_INCLUDED
#define DX11_TIMER_H_INCLUDED

//base engine class
//---
#include "Timer.h"
//--

//singleton
//---
#include"Singleton.h"
//--
class DX11Timer;
typedef Singleton<DX11Timer> TheTimer;
class DX11Timer : public Timer
{
public:
	bool Init()override;
	float TotalTime()const override;
	float DeltaTime()const override;
	
	void Tick()override;

	void Reset()override;
	void Start()override;
	void Stop()override;
private:
	DX11Timer();
	friend TheTimer;
private:
	typedef unsigned long long int int_64;
	float secondsPerCount_;
	float deltaTime_;

	int_64 baseTime_;
	int_64 pauseTime_;
	int_64 stopTime_;
	int_64 prevTime_;
	int_64 currTime_;
	bool stopped_;
};

#endif