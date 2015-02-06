#include "DX11Timer.h"

#include <D3D11.h>

DX11Timer::DX11Timer(): secondsPerCount_(0),
	deltaTime_(-1.0f),
	baseTime_(0),
	pauseTime_(0),
	prevTime_(0),
	currTime_(0),
	stopped_(false)
{
}

bool DX11Timer::Init()
{
	int_64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	secondsPerCount_=1.0f/(double)countsPerSec;
	return true;
}

float DX11Timer::DeltaTime()const
{
return deltaTime_;
}

float DX11Timer::TotalTime()const
{
 if(stopped_)
 {
 return (float)(((stopTime_-pauseTime_) -
	 baseTime_)*secondsPerCount_);
 }
 return (float)(((currTime_-pauseTime_)-
	 baseTime_)*secondsPerCount_);
}

void DX11Timer::Reset()
{
	int_64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	baseTime_=currTime;
	prevTime_=currTime;
	stopTime_=0;
	stopped_=false;
}

void DX11Timer::Tick()
{
	if(stopped_)
	{
	deltaTime_=0;
	return;
	}
	int_64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	currTime_=currTime;

	deltaTime_= (currTime_-prevTime_)*secondsPerCount_;

	prevTime_=currTime_;

	if(deltaTime_ < 0.0f)
	{
		deltaTime_=0.0f;
	}
}

void DX11Timer::Stop()
{
 if(stopped_)
 {
	int_64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	stopTime_=currTime;
	stopped_=true;
 }
}

void DX11Timer::Start()
{
	int_64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if(stopped_)
	{
		pauseTime_+=(startTime - stopTime_);
		prevTime_=startTime;
		stopTime_=0;
		stopped_=false;
	}
}