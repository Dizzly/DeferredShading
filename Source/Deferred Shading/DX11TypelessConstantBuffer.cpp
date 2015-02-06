#include "DX11TypelessConstantBuffer.h"



DX11TypelessConstantBuffer::DX11TypelessConstantBuffer()
{
	pConstantBuffer_=NULL;
	id_=-1;
	state_=BUFFER_DEAFULT;
}

DX11TypelessConstantBuffer::~DX11TypelessConstantBuffer()
{
	assert(pConstantBuffer_);
	if(pConstantBuffer_)
	{
		pConstantBuffer_->Release();
	}
}

bool DX11TypelessConstantBuffer::Init(int id, int size, BUFFER_USAGE state)
{
	id_=id;
	state_=state;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd,sizeof(bd));
	bd.Usage= (D3D11_USAGE)state_;
	bd.ByteWidth= size;
	bd.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags=0;
	HRESULT hr;
	hr=TheDX11Graphics::Instance()->GetD3DDevice()->CreateBuffer(&bd,NULL,&pConstantBuffer_);
	assert(id_!=-1);
	if(FAILED(hr))
	{
		return false;
	}
	return true;
}


void DX11TypelessConstantBuffer::Update(void* t)
{
	assert(pConstantBuffer_);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->UpdateSubresource(pConstantBuffer_,0,NULL,&t,0,0);

}

void DX11TypelessConstantBuffer::Bind(int id)
{
	if(id!=id_&&id!=-1)
	{
		id_=id;
	}
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->VSSetConstantBuffers(id_,1,&pConstantBuffer_);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->PSSetConstantBuffers(id_,1,&pConstantBuffer_);
}

