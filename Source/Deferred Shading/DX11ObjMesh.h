#ifndef DX11_OBJ_MESH_H_INCLUDED
#define DX11_OBJ_MESH_H_INCLUDED


//vertex input control for loading
//---
#include "DX11VertexInputLayout.h"
//--

//dx for buffers
//---
#include <D3D11.h>
//--

//data structures
//---
#include <string>
//--

class DX11ObjMesh
{
public:
	DX11ObjMesh();
	~DX11ObjMesh();
	bool Load(const std::string& filename,DX11VertexInputLayout& vi);
	void Draw();
private:
	DX11VertexInputLayout inputLay_;
	ID3D11Buffer* vBuffer_;
	int numOfVerts_;
};


#endif