#ifndef DX11CUBE_H_INCLUDED
#define DX11CUBE_H_INCLUDED
#include <D3DX11.h>
#include <xnamath.h>
#include "DX11Shape.h"
class DX11Cube: public DX11Shape
{
private:
	struct CubeVertsStruct;
	struct CubeIndexStruct;
public:
	DX11Cube();
	~DX11Cube();
	bool Init(DX11VertexInputLayout& vi)override;
};

#endif