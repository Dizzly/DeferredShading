#ifndef DX11_GRAPHICS_H_INCLUDED
#define DX11_GRAPHICS_H_INCLUDED
#define GAME_BUILT_IN_DX



//Direct X includes
//---
#include <Windows.h>
#include <D3D11.h>
#include <D3DX11tex.h>
//--

//Data structures
//---
#include <string>
#include "Vec2.h"
#include "Vec4.h"
//--

//Singleton
//---
#include "Singleton.h"
//--

//Base class for interface with Game
//---
#include "GraphicsManager.h"
//--


//The grpahics manager will handle the current draw state
//---
#include "DX11DrawState.h"
//--

class DX11Graphics;
typedef Singleton<DX11Graphics> TheDX11Graphics;

class DX11Graphics: public GraphicsManager
{
public://inherited functions
	~DX11Graphics();
	bool Init()override;
	bool ReInit()override;
	bool Destroy()override;
	void BeginDraw()override;
	void EndDraw()override;
	void ManageState(State*) override;
public://dx11 specific functions
	ID3D11Device* GetD3DDevice();
	ID3D11DeviceContext* GetD3DDeviceContext();
	DX11DrawState* GetCurrentDrawState();

	void SetRenderTarget(ID3D11RenderTargetView* view=NULL);
	void SetViewport(D3D11_VIEWPORT* view=NULL);
	void ClearStencil(int stencilValue);
	ID3D11DepthStencilView* GetDepthStencilView();

	void SetClearColour(Vec4f);
private://singleton stuff
	DX11Graphics();
	friend TheDX11Graphics;

private://seperate init functions
	bool InitWindow();
	bool InitSwapChain();
	bool InitDepthAndRenderViews();


private:
	DX11DrawState* drawState_;

	ID3D11Device* pd3dDevice_;
	ID3D11DeviceContext* pImmediateContext_;
	IDXGISwapChain* pSwapChain_;
	ID3D11RenderTargetView* pScreenRenderTarget_;
	ID3D11Texture2D* pDepthStencilTex_;
	ID3D11DepthStencilView* pDepthStencilView_;
private:

	Vec4f clearColour_;

	bool initialized_;
};


inline ID3D11DepthStencilView* DX11Graphics::GetDepthStencilView()
{
	return pDepthStencilView_;
}


inline ID3D11Device* DX11Graphics::GetD3DDevice()
{
	return pd3dDevice_;
}
inline ID3D11DeviceContext* DX11Graphics::GetD3DDeviceContext()
{
	return pImmediateContext_;
}

inline DX11DrawState* DX11Graphics::GetCurrentDrawState()
{
	return drawState_;
}

#endif