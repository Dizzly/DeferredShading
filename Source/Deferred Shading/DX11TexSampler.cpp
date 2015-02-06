#include "DX11TexSampler.h"

//for acess to device pointers
//---
#include "DX11Graphics.h"
//--

//error handling
//---
#include <assert.h>
//--

DX11TexSampler::DX11TexSampler()
{
id_=0;
pSampler_=NULL;
}

DX11TexSampler::~DX11TexSampler()
{
if(pSampler_)
{
	pSampler_->Release();
}
}

bool DX11TexSampler::Init(int sampleMethod, int id)
{
	HRESULT hr;
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	if(SAMPLE_LINEAR==sampleMethod)
	{
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy=1;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	}
	else
	{
		return false;
	}

	hr =TheDX11Graphics::Instance()->GetD3DDevice()->CreateSamplerState(
		&sampDesc, &pSampler_);
	if(FAILED(hr))
	{
	if(pSampler_)
	{
		pSampler_->Release();
	}
	return false;
	}
	id_=id;
	return true;
}

void DX11TexSampler::Use(int id)
{
	assert(pSampler_);
	id_=id;
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->VSSetSamplers(id_,1,&pSampler_);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->PSSetSamplers(id_,1,&pSampler_);
}



