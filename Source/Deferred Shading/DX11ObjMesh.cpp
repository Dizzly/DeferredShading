#include "DX11ObjMesh.h"




//container types
//---
#include <vector>
#include "Vec2.h" //tex coords
#include "Vec3.h" //vers and index sets
//--

//file handling and loading
//---
#include "File.h"
//--

//error handling
//---
#include <assert.h>
//--

//used for vertex buffer and index buffer
//---
#include "DX11Graphics.h"
//--

using namespace DX11InputLayout;

//static helper functions for file parsing
typedef std::vector<std::string> Strings;

static void Trim(std::string* ps)
{
	assert(ps);
	while (!ps->empty() && (ps->at(0) == ' ' || ps->at(0) == '\r' || ps->at(0) == '\n' ) )
	{
		*ps = ps->substr(1);
	}
	while (!ps->empty() && (ps->at(ps->size() - 1) == ' ' || ps->at(ps->size() - 1) == '\r' || ps->at(ps->size() - 1) == '\n') )
	{
		*ps = ps->substr(0, ps->size() - 1);
	}
}

static Strings Split(const std::string& cs, char c)
{
	Strings r;
	std::string s(cs);
	while (true) 
	{
		int i = s.find(c);
		if (i == std::string::npos)
		{
			// No special character found, so push back the entire string and finish.
			r.push_back(s);
			break; 
		}    
		else
		{
			// Found the special character. 
			// Push back whatever is before the character, then work on the remainder
			// of the string.
			r.push_back(s.substr(0, i));
			s = s.substr(i + 1);
			Trim(&s);
			if (s.empty())
			{
				break;
			}
		}
	}
	return r;
}

static float ToFloat(const std::string& s)
{
	return (float)atof(s.c_str());
}

// Convert a string to an int
static int ToInt(const std::string& s)
{
	return atoi(s.c_str());
}

static Vec2f ToVec2(const Strings& strs)
{
	if (strs.size() != 3)
	{
		// assert(0);
	}
	return Vec2f(ToFloat(strs[1]), ToFloat(strs[2]));
}

static Vec3f ToVec3(const Strings& strs)
{
	if (strs.size() != 4)
	{
		//assert(0);
	}
	return Vec3f(ToFloat(strs[1]), ToFloat(strs[2]), ToFloat(strs[3]));
}
struct  Face{
	int vertIndex[3];
	int  texIndex[3];
	int normIndex[3];
};

static Face ToFace(const Strings& fStr)
{
	Face f;
	if(fStr.size() <4)
	{
		assert(0);
	}
	int j=2;
	for(int i=0;i<3;++i)
	{
		Strings tempStr =Split(fStr[i+1],'/');
		if(tempStr.size()>0&&!tempStr[0].empty())
		{
			f.vertIndex[j]=ToInt(tempStr[0])-1;
		}
		if (tempStr.size() > 1 && !tempStr[1].empty())
		{
			f.texIndex[j] = ToInt(tempStr[1])-1;
		}
		if (tempStr.size() > 2 && !tempStr[2].empty())
		{
			f.normIndex[j] = ToInt(tempStr[2])-1;
		}
		--j;
	}
	return f;
}

DX11ObjMesh::DX11ObjMesh()
{
	vBuffer_=NULL;
	numOfVerts_=0;
}

DX11ObjMesh::~DX11ObjMesh()
{
	if(vBuffer_)vBuffer_->Release();
}

bool DX11ObjMesh::Load(const std::string& filename,DX11VertexInputLayout& vi)
{
	std::vector<Vec3f> verts;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> uvs;
	std::vector<Face> faces;

	inputLay_=vi;
	std::vector<float> vertsComplete;
	File f;
	if(!f.OpenRead(filename))
	{
		assert(0);
		return false;
	}
	std::string read;
	std::string groupName;
	while(f.GetString(&read))
	{
		Strings data(Split(read,' '));
		if('v'==data[0][0])
		{
			if("v"==data[0])
			{
				verts.push_back(ToVec3(data));
				verts.back().z*=-1;
			}
			else if('t'==data[0][1])
			{
				uvs.push_back(ToVec2(data));
				uvs.back().y =1.0f- uvs.back().y;
			}
			else if('n'==data[0][1])
			{
				normals.push_back(ToVec3(data));
				normals.back().z*=-1;
			}
		}
		else if('f'==data[0][0])
		{
			Face face= ToFace(data);
			faces.push_back(face);
		}
	}

	for(auto it=faces.begin(); it!=faces.end();++it)
	{
		Vec3f tangent;
		for(int i=0;i<3;++i)
		{
			for(int j=0;j<vi.layout.size();++j)
			{
				if(vi.layout[j]==LAYOUT_POS)
				{
					Vec3f vert =verts[it->vertIndex[i]];
					vertsComplete.push_back(vert.x);
					vertsComplete.push_back(vert.y);
					vertsComplete.push_back(vert.z);
				}
				else if(vi.layout[j]==LAYOUT_TEX)
				{
					if(uvs.size()>0)
					{
						Vec2f tex = uvs[it->texIndex[i]];
						vertsComplete.push_back(tex.x);
						vertsComplete.push_back(tex.y);
					}
					else
					{
						vertsComplete.push_back(0.0f);
						vertsComplete.push_back(0.0f);
					}
				}
				else if(vi.layout[j]==LAYOUT_NORM)
				{
					Vec3f norm = normals[it->normIndex[i]];
					vertsComplete.push_back(norm.x);
					vertsComplete.push_back(norm.y);
					vertsComplete.push_back(norm.z);
				}
				else if(vi.layout[j]==LAYOUT_TANGENT)
				{
					if(tangent==Vec3f())
					{
						Vec2f tex1=uvs[it->texIndex[0]],
							tex2=uvs[it->texIndex[1]],
							tex3=uvs[it->texIndex[2]];
						Vec3f vert1=verts[it->vertIndex[0]],
							vert2=verts[it->vertIndex[1]],
							vert3=verts[it->vertIndex[2]];

						Vec3f v1= vert2-vert1;
						Vec3f v2= vert3-vert1;

						Vec2f t1 =tex2-tex1;
						Vec2f t2= tex3-tex1;

						float r=1.0f/(t1.x*t2.y -t2.x *t1.y);

						Vec3f sDir((t2.y *v1.x - t1.y * v2.x)*r,
							(t2.y * v1.y - t1.y * v2.y)*r,
							(t2.y * v1.z - t1.y * v2.z)*r);

						Vec3f tDir((t2.x *v2.x - t1.x *v1.x)*r,
							(t2.x * v2.y - t1.x * v1.y)*r,
							(t2.x * v2.z - t1.x * v1.z)*r);
						Vec3f normal=normals[it->normIndex[0]];
						tangent =(sDir - (normal * normal.Dot(sDir)));
					}
					vertsComplete.push_back(tangent.x);
					vertsComplete.push_back(tangent.y);
					vertsComplete.push_back(tangent.z);
				}
			}
		}
	}


	numOfVerts_= vertsComplete.size()/(vi.layoutSize/sizeof(float));
	HRESULT hr;	
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertsComplete.size()*sizeof(float);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem=vertsComplete.data();
	hr = TheDX11Graphics::Instance()->GetD3DDevice()->CreateBuffer( &bd, &InitData, &vBuffer_);
	if(FAILED(hr))
	{return false;}

	return true;
}

void DX11ObjMesh::Draw()
{
	UINT stride = inputLay_.layoutSize;
	UINT offset = 0;
	assert(vBuffer_);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->IASetVertexBuffers( 0, 1, &vBuffer_, &stride, &offset );
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->Draw(numOfVerts_,0);
}