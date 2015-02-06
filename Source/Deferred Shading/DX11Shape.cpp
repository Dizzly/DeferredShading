#include "DX11Shape.h"
#include <assert.h>
#include "DX11Graphics.h"

DX11Shape::DX11Shape()
{
vBuffer_=NULL;
iBuffer_=NULL;
numOfIndex_=0;
}

DX11Shape::~DX11Shape()
{
	if(iBuffer_)iBuffer_->Release();
	if(vBuffer_)vBuffer_->Release();
}
void DX11Shape::Draw()
{
	UINT stride = inputLay_.layoutSize;
	UINT offset = 0;
	assert(vBuffer_&&iBuffer_);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->IASetVertexBuffers( 0, 1, &vBuffer_, &stride, &offset );
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->IASetIndexBuffer(iBuffer_,DXGI_FORMAT_R16_UINT,0);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->DrawIndexed(numOfIndex_,0,0);
}

void DX11Shape::LoadIndex()
{}

void DX11Shape::LoadVerts()
{}