#ifndef DX11ICOSOHEDRON_H_INCLUDED
#define DX11ICOSOHEDRON_H_INCLUDED
#include "DX11Shape.h"
#include "Vec3.h"


class DX11Icosahedron :public DX11Shape
{
public:
	DX11Icosahedron();
	~DX11Icosahedron();
	bool Init(DX11VertexInputLayout& layout)override;
	bool Init(DX11VertexInputLayout& lay, float rad, int subDiv);
private:
	void SubDiv(std::vector<Vec3f>& vert,  std::vector<WORD>& index);
private:
	float radius_;
	int subDiv_;
};


#endif