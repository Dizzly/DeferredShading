#include "WindowsWindow.h"

//error handling
//---
#include <assert.h>
#include <AntTweakBar\AntTweakBar.h>
//---


//A static callback funciton, used to process win messeges
//---
static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;
	if (TwEventWin(hWnd, message, wParam, lParam))
	{
		return 0;
	}
	switch( message )
	{
	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		EndPaint( hWnd, &ps );
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0;
}
//---

//only used once, not common enough for a member
static int g_cmdShow;

WindowsWindow::WindowsWindow()
{
hWnd_=NULL;
hInst_=NULL;
windowName_="DX11 App";
clientBounds_=Vec2i(640,480);
}

WindowsWindow::~WindowsWindow()
{}

bool WindowsWindow::Init()
{
	if(!hInst_)
	{
	assert(hInst_);
	//REPORT ERROR
	// SetHInst must be called in WinMain;
	}
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst_;
	wcex.hIcon = LoadIcon( hInst_, ( LPCTSTR )107);
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"GameWindowClass";
	wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )107);
	if( !RegisterClassEx( &wcex ) )
		return false;

	// Create window
	RECT rc = {0, 0, clientBounds_.x, clientBounds_.y };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	hWnd_ = CreateWindowA( "GameWindowClass",windowName_.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst_,
		NULL );
	if( !hWnd_ )
		return false;



	ShowWindow( hWnd_,g_cmdShow );
	SetForegroundWindow(hWnd_);
	SetFocus(hWnd_);

	return true;
}

void WindowsWindow::SetClientBounds(Vec2i size)
{
	//resizing of the window might be implemented later
	//for now, you cant change the size once the window has been setup
	if(!hWnd_)
	{
	clientBounds_=size;
	}
}

HWND WindowsWindow::GetWindowHandle()const
{
	return hWnd_;
}

HINSTANCE WindowsWindow::GetInstanceHandle()const
{
return hInst_;
}
Vec2i WindowsWindow::GetClientBounds()const
{
	return clientBounds_;
}

void WindowsWindow::SetHInst(HINSTANCE hInst, int cmd)
{
	hInst_=hInst;
	g_cmdShow=cmd;
}
