#include <Windows.h>
#include "Game.h"
#include "WindowsWindow.h"
#include "DX11Graphics.h"
#include "DX11Input.h"
#include "DX11Timer.h"
#include "MainState.h"





int WINAPI wWinMain (HINSTANCE hInst, HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine, int nCmdShow)
{

	TheWinWindow::Instance()->SetHInst(hInst,nCmdShow);
	TheWinWindow::Instance()->SetClientBounds(Vec2i(1280,648));
	TheGame::Instance()->SetGraphicsManager(TheDX11Graphics::Instance());
	TheGame::Instance()->SetInputManager(TheDX11Input::Instance());
	TheGame::Instance()->SetTimer(TheTimer::Instance());
	TheGame::Instance()->SetState(TheMainState::Instance());
	TheGame::Instance()->Init();
	TheGame::Instance()->Run();
	TheGame::Instance()->Destroy();
	return 0;
}