#include "DX11Cube.h"
#include <assert.h>
#include "DX11Graphics.h"

using namespace DX11InputLayout;

struct CubeVertsStruct
{
	XMFLOAT3 vert;
};

struct CubeNormalStruct
{
	XMFLOAT3 normal;
};
struct CubeTexStruct
{
	XMFLOAT2 tex;
};
struct CubeTangentStruct
{
	XMFLOAT3 tang;
};
struct CubeIndexStruct
{
	WORD innerIndex;
};

DX11Cube::DX11Cube()
{
}

DX11Cube::~DX11Cube()
{

}
bool DX11Cube::Init(DX11VertexInputLayout& layout)
{
	inputLay_=layout;

	XMFLOAT3 up(0,1,0);
	XMFLOAT3 down(0,-1,0);
	XMFLOAT3 right(1,0,0);
	XMFLOAT3 left(-1,0,0);
	XMFLOAT3 forwards(0,0,1);
	XMFLOAT3 back (0,0,-1);

	std::vector<float> complete;
	complete.reserve(layout.layoutSize/sizeof(float)*24);

	XMFLOAT3 n[] =
	{
		up,up,up,up,
		down,down,down,down,
		left,left,left,left,
		right,right,right,right,
		back,back,back,back,
		forwards,forwards,forwards,forwards,
	};

	XMFLOAT2 t[]={
		XMFLOAT2(0,0),XMFLOAT2(1,0),XMFLOAT2(1,1),XMFLOAT2(0,1),
		XMFLOAT2(0,0),XMFLOAT2(1,0),XMFLOAT2(1,1),XMFLOAT2(0,1),
		XMFLOAT2(0,0),XMFLOAT2(1,0),XMFLOAT2(1,1),XMFLOAT2(0,1),
		XMFLOAT2(0,0),XMFLOAT2(1,0),XMFLOAT2(1,1),XMFLOAT2(0,1),
		XMFLOAT2(0,0),XMFLOAT2(1,0),XMFLOAT2(1,1),XMFLOAT2(0,1),
		XMFLOAT2(0,0),XMFLOAT2(1,0),XMFLOAT2(1,1),XMFLOAT2(0,1),
	};

	XMFLOAT3 tan[]={
		left,left,left,left,
		left,left,left,left,
		up,up,up,up,
		down,down,down,down,
		right,right,right,right,
		left,left,left,left,
	};

	/// Array of vertices that define the cube, six faces x four corners
	XMFLOAT3 v[] = {
		XMFLOAT3(-1.0f, 1.0f, -1.0f ),
		XMFLOAT3( 1.0f, 1.0f, -1.0f ),
		XMFLOAT3( 1.0f, 1.0f, 1.0f ),
		XMFLOAT3( -1.0f, 1.0f, 1.0f ),

		XMFLOAT3( -1.0f, -1.0f, -1.0f ),
		XMFLOAT3( 1.0f, -1.0f, -1.0f ),
		XMFLOAT3( 1.0f, -1.0f, 1.0f ) ,
		XMFLOAT3( -1.0f, -1.0f, 1.0f ) ,

		XMFLOAT3( -1.0f, -1.0f, 1.0f ),
		XMFLOAT3( -1.0f, -1.0f, -1.0f ) ,
		XMFLOAT3( -1.0f, 1.0f, -1.0f ) ,
		XMFLOAT3( -1.0f, 1.0f, 1.0f ) ,

		XMFLOAT3( 1.0f, -1.0f, 1.0f ) ,
		XMFLOAT3( 1.0f, -1.0f, -1.0f ) ,
		XMFLOAT3( 1.0f, 1.0f, -1.0f ) ,
		XMFLOAT3( 1.0f, 1.0f, 1.0f ) ,

		XMFLOAT3( -1.0f, -1.0f, -1.0f ) ,
		XMFLOAT3( 1.0f, -1.0f, -1.0f ) ,
		XMFLOAT3( 1.0f, 1.0f, -1.0f ) ,
		XMFLOAT3( -1.0f, 1.0f, -1.0f ) ,

		XMFLOAT3( -1.0f, -1.0f, 1.0f ) ,
		XMFLOAT3( 1.0f, -1.0f, 1.0f ) ,
		XMFLOAT3( 1.0f, 1.0f, 1.0f ),
		XMFLOAT3( -1.0f, 1.0f, 1.0f ),
	};
	
	/// Array of indexes of vertices with which we will draw triangles.
	/// Each group of three elements will define a triangle, each element being an index into the table of vertices v defined above
	WORD index[]=
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};
	for(int i=0;i<24;++i)
	{
		int choice=i;
		for(int j=0;j<inputLay_.layout.size();++j)
		{
			if(inputLay_.layout[j]==LAYOUT_POS)
			{
				XMFLOAT3 vert=v[choice];
				complete.push_back(vert.x);
				complete.push_back(vert.y);
				complete.push_back(vert.z);
			}
			else if(inputLay_.layout[j]==LAYOUT_COL)
			{
				complete.push_back(1.0f);
				complete.push_back(1.0f);
				complete.push_back(1.0f);
				complete.push_back(1.0f);
			}
			else if(inputLay_.layout[j]==LAYOUT_TEX)
			{
				XMFLOAT2 tex= t[choice];
				complete.push_back(tex.x);
				complete.push_back(tex.y);
			}
			else if(inputLay_.layout[j]==LAYOUT_NORM)
			{
				XMFLOAT3 norm= n[choice];
				complete.push_back(norm.x);
				complete.push_back(norm.y);
				complete.push_back(norm.z);
			}
			else if(inputLay_.layout[j]==LAYOUT_TANGENT)
			{
				XMFLOAT3 tang=tan[choice];
				complete.push_back(tang.x);
				complete.push_back(tang.y);
				complete.push_back(tang.z);
			}
		}
	}

	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = inputLay_.layoutSize * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;  //Warning - this is needed because Windows initialises the flags to xxxxxx
	D3D11_SUBRESOURCE_DATA InitData;

	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem=complete.data();
	hr = TheDX11Graphics::Instance()->GetD3DDevice()->CreateBuffer( &bd, &InitData, &vBuffer_);
	if(FAILED(hr))
	{return false;}

	numOfIndex_=36;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = index;
	hr =TheDX11Graphics::Instance()->GetD3DDevice()->CreateBuffer( &bd, &InitData, &iBuffer_ );
	if( FAILED( hr ) )
	{return false;}
	return true;
}
