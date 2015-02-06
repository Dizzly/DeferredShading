#ifndef DX11SHAPE_H_INCLUDED
#define DX11SHAPE_H_INCLUDED
#include <D3DX11.h>
#include <xnamath.h>
#include "DX11VertexInputLayout.h"
class DX11Shape
{
public:
	DX11Shape();
	virtual ~DX11Shape()=0;
	virtual bool Init(DX11VertexInputLayout& vi)=0{};
	virtual void Draw();
	void LoadVerts();
	void LoadIndex();
protected:
	DX11VertexInputLayout inputLay_;
protected:
ID3D11Buffer* vBuffer_;
ID3D11Buffer* iBuffer_;
int numOfIndex_;
};

#endif