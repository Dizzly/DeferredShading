#include "DX11Quad.h"
#include <assert.h>
#include "DX11Graphics.h"

using namespace DX11InputLayout;

DX11Quad::DX11Quad()
{}
DX11Quad::~DX11Quad()
{}

bool DX11Quad::Init(DX11VertexInputLayout& layout)
{
	inputLay_=layout;

	std::vector<float> complete;
	complete.reserve(layout.layoutSize*6);

	XMFLOAT3 v[] =
	{
		XMFLOAT3(-0.5f, 0.5f, 0),
		XMFLOAT3(-0.5f, -0.5f, 0 ),
		XMFLOAT3( 0.5f, -0.5f, 0),
		


		XMFLOAT3( 0.5f, -0.5f, 0 ),
		XMFLOAT3( 0.5f, 0.5f, 0 ),
		XMFLOAT3( -0.5f, 0.5f, 0 ),	
	};

	XMFLOAT3 n[]={
		XMFLOAT3(0,0,-1),
		XMFLOAT3(0,0,-1),
		XMFLOAT3(0,0,-1),

		XMFLOAT3(0,0,-1),
		XMFLOAT3(0,0,-1),
		XMFLOAT3(0,0,-1),
	};

	XMFLOAT2 t[]={
		XMFLOAT2(0,0),
		XMFLOAT2(0,1),
		XMFLOAT2(1,1),

		XMFLOAT2(1,1),
		XMFLOAT2(1,0),
		XMFLOAT2(0,0),
	};

	XMFLOAT3 tan[]={
		XMFLOAT3(1,0,0),
		XMFLOAT3(1,0,0),
		XMFLOAT3(1,0,0),
		XMFLOAT3(1,0,0),
		XMFLOAT3(1,0,0),
		XMFLOAT3(1,0,0),
	};

	for(int i=0;i<6;++i)
	{
		for(int j=0;j<inputLay_.layout.size();++j)
		{
			if(inputLay_.layout[j]==LAYOUT_POS)
			{
				XMFLOAT3 vert=v[i];
				complete.push_back(vert.x);
				complete.push_back(vert.y);
				complete.push_back(vert.z);
			}
			if(inputLay_.layout[j]==LAYOUT_COL)
			{
				complete.push_back(1.0f);
				complete.push_back(1.0f);
				complete.push_back(1.0f);
				complete.push_back(1.0f);
			}
			if(inputLay_.layout[j]==LAYOUT_TEX)
			{
				XMFLOAT2 tex= t[i];
				complete.push_back(tex.x);
				complete.push_back(tex.y);
			}
			if(inputLay_.layout[j]==LAYOUT_NORM)
			{
				XMFLOAT3 norm= n[i];
				complete.push_back(norm.x);
				complete.push_back(norm.y);
				complete.push_back(norm.z);
			}
			if(inputLay_.layout[j]==LAYOUT_TANGENT)
			{
				XMFLOAT3 tang=tan[i];
				complete.push_back(tang.x);
				complete.push_back(tang.y);
				complete.push_back(tang.z);
			}
		}
	}
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = inputLay_.layoutSize * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = complete.data();
	hr = TheDX11Graphics::Instance()->GetD3DDevice()->CreateBuffer( &bd, &InitData, &vBuffer_ );
	return true;
}

void DX11Quad::Draw()
{
	UINT stride = inputLay_.layoutSize;
	UINT offset = 0;
	assert(vBuffer_);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->IASetVertexBuffers( 0, 1, &vBuffer_, &stride, &offset );
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->IASetIndexBuffer(iBuffer_,DXGI_FORMAT_R16_UINT,0);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->Draw(6,0);
}