#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

//General external includes.
//---
#include <stdlib.h>
//---

//Singleton includes
//---
#include "NonCopyable.h"
#include "Singleton.h"
//---

//Virtual class includes
//---
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Timer.h"
//

//State base class
//---
#include "State.h"
//---


class Game;
typedef Singleton<Game> TheGame;
class Game : public NonCopyable
{
private:
	enum BUILT_IN {BUILT_BAD=0,BUILT_DX11,BUILT_OPENGL};
public:
	~Game();
	bool Init();
	void Run();
	void Destroy();

	void SetGraphicsManager(GraphicsManager* graphics);
	void SetInputManager(InputManager* input);

	void SetState(State* state);
	State* GetState();
	void SetTimer(Timer* timer);
private:
	Game();
	friend TheGame;
	void Update();
	void Draw();

private:
	GraphicsManager* graphicsMan_;
	InputManager* inputMan_;
	Timer* timer_;
	State* curState_;

	bool isPaused_;
	bool isInit_;
	bool isRunning_;
	int builtIn_;
};

#endif

