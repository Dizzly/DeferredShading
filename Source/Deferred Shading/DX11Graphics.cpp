#include "DX11Graphics.h"

//direct x
//---
#include <d3d11.h>
//---

//Initilisation of the window
//---
#include "WindowsWindow.h"
//---

//Error checking
//---
#include <assert.h>
//---

//To standardise, Init will be used to create the window
//the showCmnd from winmain will only be used once
//so it doesnt call for a member variable
//


static bool g_isDestroyed=false;


DX11Graphics::DX11Graphics()
{
	pd3dDevice_=NULL;
	pImmediateContext_=NULL;
	pSwapChain_=NULL;
	pScreenRenderTarget_=NULL;
	pDepthStencilTex_=NULL;
	pDepthStencilView_=NULL;
	drawState_=NULL;
	initialized_=false;
	clearColour_=Vec4f(0.0f,0.0f,0.0f,1.0f);
}

DX11Graphics::~DX11Graphics()
{
	if(!g_isDestroyed)
	{
	//A reminder to destroy the object properly
	assert(0);
	}
}

bool DX11Graphics::ReInit()
{return false;}

bool DX11Graphics::Init()
{
bool win=true;
		if(!InitWindow())
		{
			//REPORT ERROR;
			assert(0);
			win =false;
		}

		if(!InitSwapChain())
		{
			//REPORT ERROR;
			assert(0);
			win=false;
		}

		if(!InitDepthAndRenderViews())
		{
			//REPORT ERROR;
			assert(0);
			win=false;
		}
initialized_=true;
return win;
}

void DX11Graphics::SetRenderTarget(ID3D11RenderTargetView* view)
{
	if(!view)
	{
		pImmediateContext_->OMSetRenderTargets(1,&pScreenRenderTarget_,pDepthStencilView_);
		return;
	}
	pImmediateContext_->OMSetRenderTargets(1,&view,pDepthStencilView_);
}

void DX11Graphics::SetViewport(D3D11_VIEWPORT* view)
{
	if(!view)
	{
	Vec2i size = TheWinWindow::Instance()->GetClientBounds();
	D3D11_VIEWPORT vp;
    vp.Width = size.x;
    vp.Height = size.y;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pImmediateContext_->RSSetViewports( 1, &vp );
	return;
	}
	pImmediateContext_->RSSetViewports( 1, view);
}

bool DX11Graphics::InitSwapChain()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect( TheWinWindow::Instance()->GetWindowHandle(), &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	//UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
	UINT createDeviceFlags = NULL;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = TheWinWindow::Instance()->GetWindowHandle();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL feature=D3D_FEATURE_LEVEL_11_0;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		D3D_DRIVER_TYPE  g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &pSwapChain_, &pd3dDevice_,&feature, &pImmediateContext_ );
		if( SUCCEEDED( hr ) )
			break;
	}
	if( FAILED( hr ) )
		return false;

	return true;
}

bool DX11Graphics::InitDepthAndRenderViews()
{
	HRESULT hr;
	Vec2i size= TheWinWindow::Instance()->GetClientBounds();

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = pSwapChain_->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	if( FAILED( hr ) )
	{
		return false;
	}

	hr = pd3dDevice_->CreateRenderTargetView( pBackBuffer, nullptr, &pScreenRenderTarget_ );
	pBackBuffer->Release();
	if( FAILED( hr ) )
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = size.x;
	descDepth.Height = size.y;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pd3dDevice_->CreateTexture2D( &descDepth, NULL, &pDepthStencilTex_ );
	if( FAILED( hr ) )
		return false;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = pd3dDevice_->CreateDepthStencilView( pDepthStencilTex_, &descDSV, &pDepthStencilView_ );
	if( FAILED( hr ) )
		return false;


	D3D11_VIEWPORT vp;
    vp.Width = size.x;
    vp.Height = size.y;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pImmediateContext_->RSSetViewports( 1, &vp );

	pImmediateContext_->OMSetRenderTargets( 1, &pScreenRenderTarget_, pDepthStencilView_ );
	return true;
}

bool DX11Graphics::InitWindow()
{
	return TheWinWindow::Instance()->Init();

}

void DX11Graphics::BeginDraw()
{
	pImmediateContext_->OMSetRenderTargets(1,&pScreenRenderTarget_,pDepthStencilView_);
	pImmediateContext_->ClearRenderTargetView( pScreenRenderTarget_, clearColour_.ToC());
	pImmediateContext_->ClearDepthStencilView( pDepthStencilView_, D3D11_CLEAR_DEPTH, 1.0f,0);
	pImmediateContext_->ClearDepthStencilView( pDepthStencilView_,D3D11_CLEAR_STENCIL,1.0f,0);
	SetViewport();
}
void DX11Graphics::EndDraw()
{
	pSwapChain_->Present(0,0);
}
bool DX11Graphics::Destroy()
{
	if(pd3dDevice_)pd3dDevice_->Release();
	if(pImmediateContext_)pImmediateContext_->Release();
	if(pSwapChain_)pSwapChain_->Release();
	if(pScreenRenderTarget_)pScreenRenderTarget_->Release();
	if(pDepthStencilTex_)pDepthStencilTex_->Release();
	if(pDepthStencilView_)pDepthStencilView_->Release();
	g_isDestroyed=true;
	return true;
}

void DX11Graphics::ManageState(State* state)
{
	DX11DrawState* draw=dynamic_cast<DX11DrawState*>(state);
	if(draw)
	{
		drawState_=draw;
		return;
	}
	drawState_=NULL;
}

void DX11Graphics::SetClearColour(Vec4f	col)
{
	clearColour_=col;
}

void DX11Graphics::ClearStencil( int s)
{
	pImmediateContext_->ClearDepthStencilView(pDepthStencilView_,D3D11_CLEAR_STENCIL,0,s);
}