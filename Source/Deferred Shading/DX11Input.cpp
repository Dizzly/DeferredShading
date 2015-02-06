#include "DX11Input.h"

//error handling
//---
#include <assert.h>
//---

//direct x inlucdes
//---
#include <D3D11.h>
//---

//Windows Window for hinstance and other variables
//---
#include "WindowsWindow.h"
//---
DX11Input::DX11Input()
{
	shouldQuit_=false;
	directInput_=NULL;
	keyboard_=NULL;
	mouse_=NULL;
}

DX11Input::~DX11Input()
{}

bool DX11Input::Init()
{
	HINSTANCE hInst= TheWinWindow::Instance()->GetInstanceHandle();
	HWND hWnd =TheWinWindow::Instance()->GetWindowHandle();
	Vec2i clientBounds_ =TheWinWindow::Instance()->GetClientBounds();

	HRESULT hr;

	hr=DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&directInput_,NULL);
	if(FAILED(hr))
	{
		assert(0);
		return false;
	}

	hr = directInput_->CreateDevice(GUID_SysKeyboard,&keyboard_,NULL);
	if(FAILED(hr))
	{
		assert(0);
		return false;
	}
	if(!InitKeyboard(hWnd))
	{
		assert(0);
		return false;
	}

	hr=directInput_->CreateDevice(GUID_SysMouse,&mouse_,NULL);
	if(FAILED(hr))
	{
		assert(0);
		return false;
	}
	if(!InitMouse(hWnd))
	{
		assert(0);
		return false;
	}
	return true;
}

bool DX11Input::InitKeyboard(HWND hWnd)
{
	HRESULT hr;
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		assert(0);
		return false;
	}
	//how the keyboard interacts with other programs
	hr = keyboard_->SetCooperativeLevel(hWnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE);
	if(FAILED(hr))
	{
		assert(0);
		return false;
	}

	hr = keyboard_->Acquire();
	if(FAILED(hr))
	{
	
		//the keyboard will attempt to be aquired every frame
	}
	return true;
}
bool DX11Input::InitMouse(HWND hWnd)
{
	HRESULT hr;
	hr = mouse_->SetDataFormat(&c_dfDIMouse);
	if(FAILED(hr))
	{
		assert(0);
		return false;
	}
	hr= mouse_->SetCooperativeLevel(hWnd,DISCL_FOREGROUND| DISCL_NONEXCLUSIVE);
	if(FAILED(hr))
	{
		assert(0);
		return false;
	}
	hr = mouse_->Acquire();
	if(FAILED(hr))
	{
	
		//the mouse will attempted to be aquired every frame
	}
	return true;
}

bool DX11Input::Destroy()
{
	if(mouse_)
	{
		mouse_->Unacquire();
		mouse_->Release();
		mouse_=NULL;
	}
	if(keyboard_)
	{
		keyboard_->Unacquire();
		keyboard_->Release();
		keyboard_=NULL;
	}
	if(directInput_)
	{
		directInput_->Release();
		directInput_=NULL;
	}
	return true;
}

void DX11Input::Update()
{
	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));
	if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if(WM_QUIT==msg.message)
	{
		shouldQuit_=true;
	}
	if(!ReadKeyboard())
	{
		assert(0);
	}
	if(!ReadMouse())
	{
		assert(0);
	}
	ProcessMouse();

}

bool DX11Input::ReadKeyboard()
{
	HRESULT hr;
	hr= keyboard_->GetDeviceState(sizeof(keysState_),(LPVOID)&keysState_);
	if(FAILED(hr))
	{
		if((DIERR_INPUTLOST==hr)||(DIERR_NOTACQUIRED==hr))
		{
			keyboard_->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool DX11Input::ReadMouse()
{
	HRESULT hr;
	hr=mouse_->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState_);
	if(FAILED(hr))
	{
		if((DIERR_INPUTLOST==hr)||(DIERR_NOTACQUIRED==hr))
		{
			mouse_->Acquire();
		}
		else
		{
		return false;
		}
	}
	return true;
}

void DX11Input::ProcessMouse()
{
Vec2i bounds= TheWinWindow::Instance()->GetClientBounds();
mousePos_.x+= mouseState_.lX;
mousePos_.y+= mouseState_.lY;

if(mousePos_.x <0){mousePos_.x=0;}
if(mousePos_.y <0){mousePos_.y=0;}

if(mousePos_.x>bounds.x){mousePos_.x=bounds.x;}
if(mousePos_.y>bounds.y){mousePos_.y=bounds.y;}
}

Vec2i DX11Input::GetMousePos()const
{
return mousePos_;
}

bool DX11Input::IsKeyDown(int key)
{
	if(keysState_[key] & 0x80)
	return true;

	return false;
}

bool DX11Input::IsKeyUp(int key)
{
	if(!(keysState_[key] & 0x80))
	return true;

	return false;
}

inline bool DX11Input::ShouldQuit()
{
	return shouldQuit_;
}


