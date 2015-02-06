#include "DXGBuffer.h"



//debuging
//---
#include <assert.h>
//--

//necessary for screen size and binding commnads
//---
#include "DX11Graphics.h"
#include "WindowsWindow.h"
//--


DXGBuffer::DXGBuffer()
{}

DXGBuffer::~DXGBuffer()
{}

bool DXGBuffer::Init()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC targetDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
	
	HRESULT r;

	Vec2i screenSize=TheWinWindow::Instance()->GetClientBounds();

	ZeroMemory(&textureDesc,sizeof(textureDesc));

	textureDesc.Width=screenSize.x;
	textureDesc.Height=screenSize.y;
	textureDesc.MipLevels =1;
	textureDesc.ArraySize=1;
	textureDesc.Format=DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage=D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags=D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags=0;
	textureDesc.MiscFlags=0;


	targetDesc.Format=textureDesc.Format;
	targetDesc.ViewDimension=D3D11_RTV_DIMENSION_TEXTURE2D;
	targetDesc.Texture2D.MipSlice=0;

	resourceDesc.Format=textureDesc.Format;
	resourceDesc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MipLevels=1;
	resourceDesc.Texture2D.MostDetailedMip=0;


	for(int i=0;i<BUFF_SIZE;++i)
	{
		r = TheDX11Graphics::Instance()->GetD3DDevice()->CreateTexture2D(&textureDesc,NULL,
			&texes_[i]);
		if(FAILED(r))
		{
			assert(0);
			return false;
		}
		r=TheDX11Graphics::Instance()->GetD3DDevice()->CreateRenderTargetView(texes_[i],&targetDesc,
			&renderTarget_[i]);
		if(FAILED(r))
		{
			assert(0);
			return false;
		}
		r=TheDX11Graphics::Instance()->GetD3DDevice()->CreateShaderResourceView(texes_[i],&resourceDesc,
			&texViews_[i]);
		if(FAILED(r))
		{
			assert(0);
			return false;
		}
	}

	if(FAILED(r))
	{
		assert(0);
		return false;
	}

	viewport_.Width=screenSize.x;
	viewport_.Height=screenSize.y;
	viewport_.MinDepth=0.0f;
	viewport_.MaxDepth=1.0f;
	viewport_.TopLeftX=0;
	viewport_.TopLeftY=0;

	return true;
}

void DXGBuffer::Shutdown()
{
	for(int i=0;i<BUFF_SIZE;++i)
	{
		if(texes_[i])
		{
		texes_[i]->Release();
		texes_[i]=0;
		}
		if(renderTarget_[i])
		{
		renderTarget_[i]->Release();
		renderTarget_[i]=0;
		}
		if(texViews_[i])
		{
		texViews_[i]->Release();
		texViews_[i]=0;
		}
	}
}

void DXGBuffer::BindToWrite()
{
	Clear();
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetRenderTargets(BUFF_SIZE,
		&renderTarget_[0],TheDX11Graphics::Instance()->GetDepthStencilView());

	TheDX11Graphics::Instance()->GetD3DDeviceContext()->RSSetViewports(1,&viewport_);
}

void DXGBuffer::BindToRead(int start)
{
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->PSSetShaderResources(start,BUFF_SIZE,
		&texViews_[0]);
}

ID3D11Texture2D* DXGBuffer::GetTex(int i)
{
return texes_[i];
}

void DXGBuffer::Clear()
{
	float color[4]={0,0,0,0};

	for(int i=0;i<BUFF_SIZE;++i)
	{
		TheDX11Graphics::Instance()->GetD3DDeviceContext()->ClearRenderTargetView(renderTarget_[i],color);
	}

}
