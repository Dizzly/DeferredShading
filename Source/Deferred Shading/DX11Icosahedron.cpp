#include "DX11Icosahedron.h"
#include "DX11Graphics.h"

using namespace DX11InputLayout;

DX11Icosahedron::DX11Icosahedron()
{
	radius_=1.0f;
	subDiv_=0;
}

DX11Icosahedron::~DX11Icosahedron()
{

}

bool DX11Icosahedron::Init(DX11VertexInputLayout& lay, float rad, int subDiv)
{
	if(rad>0)
	{
	radius_=rad;
	}

	if(subDiv>0)
	{
	subDiv_=subDiv;
	}
	return Init(lay);
}
bool DX11Icosahedron::Init(DX11VertexInputLayout& layout)
{

	inputLay_=layout;
	float x= 0.525731112119133606f;
	float z= 0.850650808352039932f;

	//12 base verts
	//60 base index
	//20 base faces
	//each face turns into 4 with 1 level of subdivision
	//adding 3 verts per face recursively

	int totalFaces= 20;
	if(subDiv_>0)
	{
	totalFaces = 20* pow(4.0f,(float)subDiv_);
	}

	int test1 = pow(4.0f,1);
	int test2 = pow(4.0f,2);

	int test3 = 10*pow(1.0f,2)+2;
	int test4 =10*pow(2.0f,2)+2;
	int test5 = 10*pow(3.0f,2)+2; //http://wackel.home.comcast.net/~wackel/Geometry/GeoSubdivision.html
			// helped me figure out the calculation for vertex numbers after n subdivisions


	int totalVerts=12;
	if(subDiv_>0)
	{
		totalVerts=10*(pow((float)subDiv_,2)+5);
	}
	
	std::vector<float> complete;
	complete.reserve((layout.layoutSize*60)*(1+subDiv_));

	std::vector<Vec3f> v;
	v.reserve(totalVerts);
	v.push_back(Vec3f(-x,0.0f,z));
	v.push_back(Vec3f(x,0.0f,z));
	v.push_back(Vec3f(-x,0.0f,-z));
	v.push_back(Vec3f(x,0.0f,-z));
	v.push_back(Vec3f(0.0f,z,x));
	v.push_back(Vec3f(0.0f,z,-x));
	v.push_back(Vec3f(0.0f,-z,x));
	v.push_back(Vec3f(0.0f,-z,-x));
	v.push_back(Vec3f(z,x,0.0f));
	v.push_back(Vec3f(-z,x,0.0f));
	v.push_back(Vec3f(z,-x,0.0f));
	v.push_back(Vec3f(-z,-x,0.0f));
	
	
	WORD indexData[]={
		1,4,0, 
		4,9,0, 
		4,5,9, 
		8,5,4, 
		1,8,4,
		1,10,8, 
		10,3,8, 
		8,3,5, 
		3,2,5, 
		3,7,2,
		3,10,7, 
		10,6,7, 
		6,11,7, 
		6,0,11, 
		6,1,0,
		10,1,6, 
		11,0,9, 
		2,11,9, 
		5,2,9, 
		11,2,7
	};

	std::vector<WORD> index;
	index.reserve(totalFaces*3);
	for(int i=0;i<60;++i)
	{
	index.push_back(indexData[i]);
	}

	for(int i=0;i<subDiv_;++i)
	{
		SubDiv(v,index);
	}

	for(size_t i=0;i<v.size();++i)
	{
		for(size_t j=0;j<inputLay_.layout.size();++j)
		{
			if(inputLay_.layout[j]==LAYOUT_POS)
			{
				Vec3f vert=v[i];
				vert.Normalise();
				complete.push_back(vert.x*radius_);
				complete.push_back(vert.y*radius_);
				complete.push_back(vert.z*radius_);
			}
			else if(inputLay_.layout[j]==LAYOUT_NORM)
			{
				Vec3f vert=v[i];
				vert.Normalise();
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
			else
			{
			
				float theta = atan2(v[i].x,v[i].z)+XM_PI;//look at lunas implementation of the AngleFromXY helper function.
				float phi = acosf(v[i].y/radius_);

				if(inputLay_.layout[j]==LAYOUT_TEX)
				{
					complete.push_back(theta/XM_2PI);
					complete.push_back(phi/XM_PI);
				}
				else if(inputLay_.layout[j]==LAYOUT_TANGENT)
				{
					complete.push_back(-radius_*sinf(phi)*sinf(theta));
					complete.push_back(0.0f);
					complete.push_back(radius_*sinf(phi)*cosf(theta));
				}
			}
		}
	}

	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = inputLay_.layoutSize * v.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem=complete.data();
	hr = TheDX11Graphics::Instance()->GetD3DDevice()->CreateBuffer( &bd, &InitData, &vBuffer_);
	if(FAILED(hr))
	{return false;}

	numOfIndex_=index.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * index.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = index.data();
	hr = TheDX11Graphics::Instance()->GetD3DDevice()->CreateBuffer( &bd, &InitData, &iBuffer_ );
	if( FAILED( hr ) )
	{return false;}
	return true;

}

void DX11Icosahedron::SubDiv(std::vector<Vec3f>& vert, std::vector<WORD>& index)
{
	std::vector<Vec3f> newVert;
	newVert.reserve(vert.size());
	newVert=vert;

	std::vector<WORD> newIn;
	newIn.reserve(index.size());
	for(size_t i=0;i<index.size();i+=3)
	{
		int iA=index[i];
		int iB=index[i+1];
		int iC=index[i+2];

		Vec3f vertA=vert[iA];
		Vec3f vertB=vert[iB];
		Vec3f vertC=vert[iC];
		
		Vec3f aToB =vertB-vertA;
		Vec3f bToC =vertC-vertB;
		Vec3f cToA =vertA-vertC;

		Vec3f newPointA=vertA+(aToB*0.5);
		Vec3f newPointB=vertB+(bToC*0.5);
		Vec3f newPointC=vertC+(cToA*0.5);

		int indexStart=newVert.size();
		newVert.push_back(newPointA);
		newVert.push_back(newPointB);
		newVert.push_back(newPointC);

		newIn.push_back(iA);
		
		newIn.push_back(indexStart);
		newIn.push_back(indexStart+2);

		newIn.push_back(indexStart);
		
		newIn.push_back(iB);
		newIn.push_back(indexStart+1);

		newIn.push_back(indexStart+2);
		
		newIn.push_back(indexStart+1);
		newIn.push_back(iC);

		newIn.push_back(indexStart+2);
		
		newIn.push_back(indexStart);
		newIn.push_back(indexStart+1);
	}
	vert=newVert;
	index=newIn;
}