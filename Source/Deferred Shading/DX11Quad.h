#ifndef DX11QUAD_H_INCLUDED
#define DX11QUAD_H_INCLUDED
#include "DX11Shape.h"
#include "DX11VertexInputLayout.h"
class DX11Quad:public DX11Shape
{
public:
	DX11Quad();
	~DX11Quad();
	bool Init(DX11VertexInputLayout& layout)override;
	void Draw()override;
private:

};


#endif