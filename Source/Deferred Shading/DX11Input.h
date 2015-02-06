#ifndef DX11_INPUT_H_INCLUDED
#define DX11_INPUT_H_INCLUDED


//Direct input managment for DX
//---
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
//---

//Singleton
//---
#include "Singleton.h"
//---

//Base class for Game input
//---
#include "InputManager.h"
//---

//Data structures
//---
#include "Vec2.h"
//---


class DX11Input;
typedef Singleton<DX11Input> TheDX11Input;

class DX11Input:public InputManager
{
public:
	~DX11Input();
	virtual bool Init()override;
	virtual bool Destroy()override;
	virtual void Update()override;

	virtual bool ShouldQuit()override;

public:
	Vec2i GetMousePos()const;

	bool IsKeyDown(int);
	bool IsKeyUp(int);

private://singleton stuff
	DX11Input();
	friend TheDX11Input;

private:
	bool InitKeyboard(HWND);
	bool InitMouse(HWND);

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessMouse();
private:
	IDirectInput8* directInput_;
	IDirectInputDevice8* keyboard_;
	IDirectInputDevice8* mouse_;

	unsigned char keysState_[256];
	DIMOUSESTATE mouseState_;
	Vec2i mousePos_;

	bool shouldQuit_;
};

#endif