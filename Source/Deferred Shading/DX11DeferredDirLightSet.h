#pragma once
#include "DX11ConstantBuffer.h"
#include "DX11Shader.h"
#include "DX11Quad.h"
#include "DX11Graphics.h"
#include "DX11Transform.h"

#include "DirectionalLight.h"





class DX11DeferredDirLightSet
{
private:
	typedef std::vector<DirectionalLight> ActiveDir;
public:
	bool Init(
		int bindSlot,
		DX11Shader& shader);

	void Render();

	bool AddLight(DirectionalLight& light);

	void PushLight(DirectionalLight& light);
	void Clear();


private:
	int bindSlot_;
	DX11Shader* shader_;

	ActiveDir activeDir_;
	DX11ConstantBuffer<DirectionalLight> lightBuffer_;
	DX11Quad quad_;
	ID3D11BlendState* blend_;

	DX11Shader nullShader_;
	ID3D11RasterizerState * stencilRun_;
	ID3D11DepthStencilState * depthStencilState_;
	ID3D11DepthStencilState* lightStencilState_;
};