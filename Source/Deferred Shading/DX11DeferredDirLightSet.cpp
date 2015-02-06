#include "DX11DeferredDirLightSet.h"
#include "DX11VertexInputLayout.h"

bool DX11DeferredDirLightSet::Init(int bindSlot,DX11Shader& shader)
{
	bindSlot_=bindSlot;
	lightBuffer_.Init(bindSlot_);


	DX11VertexInputLayout vs;
	vs.layout.push_back(DX11InputLayout::LAYOUT_POS);
	vs.layoutSize=sizeof(float)*3;

	shader_=&shader;

	quad_.Init(shader_->GetLayout());


	D3D11_BLEND_DESC bs;
	ZeroMemory(&bs,sizeof(bs));
	bs.RenderTarget[0].BlendEnable=true;
	bs.RenderTarget[0].SrcBlend=D3D11_BLEND_ONE;
	bs.RenderTarget[0].DestBlend=D3D11_BLEND_ONE;
	bs.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
	bs.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ONE;
	bs.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ZERO;
	bs.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;

	bs.RenderTarget[0].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;


	TheDX11Graphics::Instance()->GetD3DDevice()->CreateBlendState(&bs,&blend_);
	/*
	nullShader_.Load("NullShader.fx",vs,"VS","PS");
	D3D11_RASTERIZER_DESC rs;
	ZeroMemory(&rs,sizeof(rs));
	rs.CullMode = D3D11_CULL_NONE;
	rs.FrontCounterClockwise=true;
	rs.DepthClipEnable=true;
	rs.ScissorEnable=true;
	rs.FillMode =D3D11_FILL_SOLID;
	TheDX11Graphics::Instance()->GetD3DDevice()->CreateRasterizerState(
	&rs,&stencilRun_);
	*/

	D3D11_DEPTH_STENCIL_DESC ds;
	ds.DepthEnable=true;
	ds.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;
	ds.DepthFunc=D3D11_COMPARISON_LESS ;

	ds.StencilEnable=true;
	ds.StencilReadMask=0xFF;
	ds.StencilWriteMask=0xFF;

	ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	ds.FrontFace.StencilFunc= D3D11_COMPARISON_ALWAYS;

	ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	ds.BackFace.StencilFunc =D3D11_COMPARISON_ALWAYS;


	TheDX11Graphics::Instance()->GetD3DDevice()->CreateDepthStencilState(&ds,
		&depthStencilState_);
	/*
	ZeroMemory(&ds,sizeof(ds));
	ds.DepthEnable=true;
	ds.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;
	ds.DepthFunc=D3D11_COMPARISON_LESS;

	ds.StencilEnable=true;
	ds.StencilReadMask=0xFF;
	ds.StencilWriteMask=0xFF;

	ds.FrontFace.StencilFailOp= D3D11_STENCIL_OP_ZERO;
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	ds.FrontFace.StencilPassOp= D3D11_STENCIL_OP_KEEP;

	TheDX11Graphics::Instance()->GetD3DDevice()->CreateDepthStencilState(&ds,
	&lightStencilState_);
	//requires the depth test to be enabled but depth writing to be disabled
	//check frank d luna on Direct x rendering to  see if this is possible.
	*/
	return true;
}

void DX11DeferredDirLightSet::Render()
{
	if(activeDir_.size()>0)
	{
		lightBuffer_.Bind(bindSlot_);
		float a[4]={0,0,0,0};
		shader_->Use();
		TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetBlendState(blend_,a,0xffffffff);
		TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetDepthStencilState(depthStencilState_,0);


		for(unsigned int i=0;i<activeDir_.size();++i)
		{
			lightBuffer_.Update(activeDir_[i]);
			quad_.Draw();
		}
		TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetBlendState(0,0,0xffffffff);
		TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetDepthStencilState(0,0);
	}
}




bool DX11DeferredDirLightSet::AddLight(DirectionalLight& light)
{

	activeDir_.push_back(light);
	return true;
}

void DX11DeferredDirLightSet::PushLight(DirectionalLight& light)
{
	activeDir_.push_back(light);
}


void DX11DeferredDirLightSet::Clear()
{
	activeDir_.clear();
}