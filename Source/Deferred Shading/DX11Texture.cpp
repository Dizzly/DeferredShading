#include "DX11Texture.h"
#include <assert.h>
#include "DX11Graphics.h"

DX11Texture::DX11Texture()
{
	pTexture_=NULL;
}

DX11Texture::~DX11Texture()
{
	if(pTexture_)pTexture_->Release();
}

bool DX11Texture::Load(const std::string& file,int id)
{
	HRESULT hr=NULL;
	id_=id;
	hr= D3DX11CreateShaderResourceViewFromFileA(TheDX11Graphics::Instance()->GetD3DDevice(),file.data(),NULL,NULL, &pTexture_,NULL);
	if(FAILED(hr))
	{assert(0);
	return false;}
	return true;
}

void DX11Texture::SetID(int id)
{id_=id;}

int DX11Texture::GetID()const
{return id_;}

void DX11Texture::Use(int id)
{
	id_=id;
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->PSSetShaderResources(id_, 1, &pTexture_);
}

void DX11Texture::Stop()
{
	ID3D11ShaderResourceView* reset[1]={0};
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->PSSetShaderResources(id_,1,reset);
}