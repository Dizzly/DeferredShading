#ifndef WINDOWS_WINDOW_H_INCLUDED
#define WINDOWS_WINDOW_H_INCLUDED

//windows includes
//---
#include <Windows.h>
#include <WinError.h>
//---

//singleton
//---
#include "NonCopyable.h"
#include "Singleton.h"
//---

//data structures
//---
#include "Vec2.h"
#include <string>
//---
class WindowsWindow;
typedef Singleton<WindowsWindow> TheWinWindow;

class WindowsWindow
{
public:
	~WindowsWindow();
	bool Init();
	void SetHInst(HINSTANCE hinst, int CmndShow);
public:
	void SetClientBounds(Vec2i);

	HWND GetWindowHandle()const;
	HINSTANCE GetInstanceHandle()const;

	Vec2i GetClientBounds()const;
private://singleton stuff
		WindowsWindow();
		friend TheWinWindow;
private:
	HINSTANCE hInst_;
	HWND hWnd_;
	std::string windowName_;

	Vec2i clientBounds_;
};



#endif