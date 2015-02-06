#pragma once;
#include <D3DX11.h>
#include <Windows.h>
#include <assert.h>
#include "DX11Graphics.h"

template<class T>
class DX11ConstantBuffer
{
public:
	enum BUFFER_USAGE {BUFFER_DEAFULT=0,BUFFER_DYNAMIC=2};
public:
	//----------
	DX11ConstantBuffer()
	{
		pConstantBuffer_=NULL;
		id_=-1;
		state_=BUFFER_DEAFULT;
	}
	//----------
	bool Init(int id,BUFFER_USAGE state=BUFFER_DEAFULT)
	{
		id_=id;
		state_=state;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.Usage= (D3D11_USAGE)state_;
		bd.ByteWidth= sizeof(T);
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
	//----------
	~DX11ConstantBuffer()
	{
		if(pConstantBuffer_)
		{
			pConstantBuffer_->Release();
		}
	}
	//----------
	void Update(T& t)
	{
		assert(pConstantBuffer_);
		TheDX11Graphics::Instance()->GetD3DDeviceContext()->UpdateSubresource(pConstantBuffer_,0,NULL,&t,0,0);

	}
	//----------
	void Bind(int id=-1)
	{
		if(id!=id_&&id!=-1)
		{
			id_=id;
		}
		TheDX11Graphics::Instance()->GetD3DDeviceContext()->VSSetConstantBuffers(id_,1,&pConstantBuffer_);
		TheDX11Graphics::Instance()->GetD3DDeviceContext()->PSSetConstantBuffers(id_,1,&pConstantBuffer_);
	}
public:

private:
	ID3D11Buffer* pConstantBuffer_;
	BUFFER_USAGE state_;
	int id_;
};