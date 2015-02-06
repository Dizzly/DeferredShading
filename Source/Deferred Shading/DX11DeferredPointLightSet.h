#pragma once


#include "DX11ConstantBuffer.h"
#include "DX11Shader.h"
#include "DX11Icosahedron.h"
#include "DX11Graphics.h"
#include "DX11Transform.h"

#include "PointLight.h"
#include "DX11Camera.h"

class DX11DeferredPointLightSet
{
private:
	typedef std::vector<PointLight> ActiveDir;
public:
	bool Init(int bindSlot,DX11Shader& shader);
	void Render(DX11Camera& c,int num=-1);
	bool AddLight(PointLight& light);

	void PushLight(PointLight& light);

	void SetAttenuationAll(Vec3f att);

	void Clear();

private:
	int bindSlot_;//dont need to keep this, but is quite useful
	DX11Shader* shader_; // the deferred light shader

	ActiveDir activeDir_;
	DX11ConstantBuffer<PointLight> lightBuffer_;
	DX11Icosahedron sphere_;
	ID3D11BlendState* blend_;

	DX11Shader nullShader_;
	ID3D11RasterizerState* stencilRun_;
	ID3D11RasterizerState* frontCull_;
	ID3D11DepthStencilState* firstPass_;
	ID3D11DepthStencilState* secondPass_;
};