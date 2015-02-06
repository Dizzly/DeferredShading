#pragma once

//base class
//---
#include "DX11DrawState.h"
//--

//DX11 objects
//---
#include "DX11ConstantBuffer.h"
#include "DX11Icosahedron.h"
#include "DX11Shader.h"
#include "DX11Camera.h"
#include "DX11Quad.h"
#include "DX11Texture.h"
#include "DX11ObjMesh.h"
#include "DX11Cube.h"
#include "DX11LightSet.h"
//--


//Deferred Specific Objects
//---
#include "DXGBuffer.h"
#include "DX11DeferredDirLightSet.h"
#include "DX11DeferredPointLightSet.h"
#include "DX11LightSet.h"
//--

//Ant Tweak Bar
//---
#include <AntTweakBar\AntTweakBar.h>
//--


class MainState;
typedef Singleton<MainState> TheMainState;
class MainState:public DX11DrawState
{
public: 
	~MainState();
	virtual void Update()override;
	virtual void Draw()override;
	virtual void OnActive()override;
	virtual void OnInactive()override;
	virtual void BindWorld()override;
	virtual void BindView()override;
	virtual void BindProj()override;

	virtual void ClearResources()override;
private:
	MainState();
	friend TheMainState;
private:
	struct WorldStruct {
		WorldStruct(XMMATRIX& aWorld, XMMATRIX& aInverse):world(aWorld),inverse(aInverse)
		{}
		XMMATRIX world;
		XMMATRIX inverse;
	};
	struct ViewStruct{
		ViewStruct(XMMATRIX& aView, XMVECTOR& aEyePos):view(aView), eyePos(aEyePos)
		{}
		XMMATRIX view;
		XMVECTOR eyePos;
	};
private:
	void DrawSimpleScene();
	void DrawNormalScene();
	void DrawComplexScene();
	void DrawPrettyScene();
private:
	DX11Texture wood_;
	DX11Texture tRexSkin_;
	DX11Texture white_;
	DX11ConstantBuffer<WorldStruct> worldBuf_;
	DX11ConstantBuffer<ViewStruct> viewBuf_;
	DX11ConstantBuffer<XMMATRIX> projBuf_;

	DX11Camera cam_;
	DX11Camera cam2d_;

	DX11DeferredDirLightSet dirSet_;
	DX11DeferredPointLightSet pointSet_;
	DX11Shader shader_;
	DX11Shader pointLightShader_;
	DX11Shader preDeff_;
	DX11Shader dirLightShader_;
	DXGBuffer gBuffer_;

	DX11Shader forwardShader_;
	DX11LightSet<160,16,540,16> forwardSet_;

	DX11Quad quad_;
	DX11Cube cube_;
	DX11Icosahedron ball_;
	DX11ObjMesh mesh_;

	D3D11_VIEWPORT topLeft_;
	D3D11_VIEWPORT topRight_;
	D3D11_VIEWPORT botRight_;
	D3D11_VIEWPORT botLeft_;

	TwBar * bar_;

	float waitTimer_;
	float startTime_;
	int sceneChoice_;
};