#include "Game.h"

//Error checking
//---
#include <assert.h>
//---

Game::Game()
{
	isRunning_=false;
	isInit_=false;
	graphicsMan_=NULL;
	inputMan_=NULL;
#ifdef GAME_BUILT_IN_DX
	builtIn_=BUILT_DX11;
#endif
#ifdef GAME_BUILT_IN_GL
	builtIn_=BUILT_OPENGL;
#endif
}

Game::~Game()
{

}

bool Game::Init()
{
	if(!isRunning_)
	{
		if(!graphicsMan_)
		{
			assert(graphicsMan_);
			//check the defines, load DX11 Graphics
		}
		if(!graphicsMan_->Init())
		{
			assert(0);
		}
		if(!inputMan_)
		{
			assert(inputMan_);
			//check the defines, load DX11 input
		}
		if(!inputMan_->Init())
		{
			assert(0);
		}
		if(!timer_)
		{
			assert(timer_);
			// check the defines, load timer
		}
		if(!timer_->Init())
		{
			assert(0);
		}
	}
	isInit_=true;
	return true;
}

void Game::Run()
{
	assert(curState_);
	SetState(curState_);
	assert(timer_);
	timer_->Reset();

	bool run=true;
	isRunning_=true;
	while(run)
	{
		inputMan_->Update();
		if(!inputMan_->ShouldQuit())
		{
			Update();
			Draw();
			continue;
		}
		else
		{
			run=false;
		}
	}
	return;
}

void Game::SetState(State* state)
{
	if(state)
	{
		if(curState_)
		{
			curState_->OnInactive();
		}
		curState_=state;
		if(isInit_)
		{
			graphicsMan_->ManageState(curState_);
			curState_->OnActive();
		}
	}
}

State* Game::GetState()
{
	if(curState_)
	{
	return curState_;
	}
	return NULL;
}

void Game::SetTimer(Timer* timer)
{
timer_=timer;
}

void Game::Update()
{
	if(curState_)
	{
		timer_->Tick();
		curState_->Update();
	}
}

void Game::Draw()
{
	graphicsMan_->BeginDraw();
	if(curState_)
	{
		curState_->Draw();
	}
	graphicsMan_->EndDraw();
}

void Game::Destroy()
{
	if(graphicsMan_)
	{
		graphicsMan_->Destroy();
	}
	if(inputMan_)
	{
		inputMan_->Destroy();
	}
}

void Game::SetGraphicsManager(GraphicsManager* graphics)
{
	if(!isRunning_)
	{
		graphicsMan_=graphics;
	}
}

void Game::SetInputManager(InputManager* input)
{
	if(!isRunning_)
	{
		inputMan_=input;
	}
}