#include "DX11DeferredPointLightSet.h"
#include <assert.h>


bool DX11DeferredPointLightSet::Init(
		int bindSlot,
		DX11Shader& shader)
	{

		bindSlot_=bindSlot;
		lightBuffer_.Init(bindSlot_);


		DX11VertexInputLayout vs;
		vs.layout.push_back(DX11InputLayout::LAYOUT_POS);
		vs.layoutSize=sizeof(float)*3;
		shader_=&shader;

		sphere_.Init(shader_->GetLayout(),1,2);
		nullShader_.Load("NullShader.fx",vs,"VS","PS");


		HRESULT r;


		//---BLEND STATE
		D3D11_BLEND_DESC bs;
		ZeroMemory(&bs,sizeof(bs));
		bs.RenderTarget[0].BlendEnable=true;
		bs.RenderTarget[0].SrcBlend=D3D11_BLEND_ONE; //used for accumilating lighting for the final frame
		bs.RenderTarget[0].DestBlend=D3D11_BLEND_ONE;
		bs.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
		bs.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ONE;
		bs.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ZERO;
		bs.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
		bs.RenderTarget[0].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;

		r=TheDX11Graphics::Instance()->GetD3DDevice()->CreateBlendState(&bs,&blend_);
		if(FAILED(r))
		{
			assert(0);
			return false;
		}
		///---

		///----RASTERIZER STATES
		// NO CULLING
		D3D11_RASTERIZER_DESC rs;
		ZeroMemory(&rs,sizeof(rs));
		rs.CullMode=D3D11_CULL_NONE;
		rs.FrontCounterClockwise=false;
		rs.DepthClipEnable=true;
		rs.FillMode=D3D11_FILL_SOLID;
		r=TheDX11Graphics::Instance()->GetD3DDevice()->CreateRasterizerState(
			&rs,&stencilRun_);
		if(FAILED(r))
		{
			assert(0);
			return false;
		}
		//FRONT CULLING
		rs.CullMode=D3D11_CULL_FRONT;

		r=TheDX11Graphics::Instance()->GetD3DDevice()->CreateRasterizerState(
			&rs,&frontCull_);
		if(FAILED(r))
		{
			assert(0);
			return false;
		}
		///---

		///--- STENCIL STATES
		// FIRST RUN
		D3D11_DEPTH_STENCIL_DESC ds;
		ds.DepthEnable=true;
		ds.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;
		ds.DepthFunc=D3D11_COMPARISON_LESS;

		ds.StencilEnable=true;
		ds.StencilReadMask=D3D11_DEFAULT_STENCIL_READ_MASK;
		ds.StencilWriteMask=D3D11_DEFAULT_STENCIL_WRITE_MASK;

		ds.FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
		ds.FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_DECR;
		ds.FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;
		ds.FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;

		ds.BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
		ds.BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_INCR;
		ds.BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;
		ds.BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;

		r=TheDX11Graphics::Instance()->GetD3DDevice()->CreateDepthStencilState(&ds,
			&firstPass_);
		if(FAILED(r))
		{
			assert(0);
			return false;
		}
		//SECOND RUN
		ds.DepthEnable=false;
		ds.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;

		ds.FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;  //should never be operated
		ds.FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;//front face culling enabled
		ds.FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;
		ds.FrontFace.StencilFunc=D3D11_COMPARISON_NEVER;

		ds.BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
		ds.BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;
		ds.BackFace.StencilPassOp=D3D11_STENCIL_OP_ZERO;
		ds.BackFace.StencilFunc=D3D11_COMPARISON_NOT_EQUAL;
		r=TheDX11Graphics::Instance()->GetD3DDevice()->CreateDepthStencilState(&ds,
			&secondPass_);
		if(FAILED(r))
		{
			assert(0);
			return false;
		}
		return true;
	}
	void DX11DeferredPointLightSet::Render(
		DX11Camera& c,
		int renderNumLights)
	{
		if (renderNumLights == -1)
		{
			renderNumLights = activeDir_.size();
		}
		if(activeDir_.size()>0)
		{
			lightBuffer_.Bind();
			DX11Camera* oldCam=TheDX11Graphics::Instance()->GetCurrentDrawState()->GetCam();

			float a[4]={0,0,0,0};
			c.Use();
			TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetBlendState(blend_,a,0xffffffff);

			for(unsigned int i=0;i<renderNumLights;++i)
			{
				DX11Transform t;

				t.Translate(activeDir_[i].GetPos());
				float maxChannel = max(max(activeDir_[i].GetDiffuse().x, activeDir_[i].GetDiffuse().y),activeDir_[i].GetDiffuse().z);
				float attSize = (-activeDir_[i].GetAttenuation().y + sqrtf(activeDir_[i].GetAttenuation().y*activeDir_[i].GetAttenuation().y -
					4 * activeDir_[i].GetAttenuation().z *(activeDir_[i].GetAttenuation().z - 256 * maxChannel * 0.1)
					/
					2 * activeDir_[i].GetAttenuation().z
					));

				float size = activeDir_[i].GetRange();
					t.Scale(Vec3f(attSize,
					attSize,
					attSize));
				t.AddUse();


				TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetDepthStencilState(firstPass_,0);
				TheDX11Graphics::Instance()->GetD3DDeviceContext()->RSSetState(stencilRun_);

				//dont have to clear because of stencil opp Zero

				nullShader_.Use();
				sphere_.Draw();

				TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetDepthStencilState(secondPass_,0);
				TheDX11Graphics::Instance()->GetD3DDeviceContext()->RSSetState(frontCull_);

				lightBuffer_.Update(activeDir_[i]);
				shader_->Use();
				sphere_.Draw();


				t.Stop();
			}
			TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetBlendState(0,0,0xffffffff);
			TheDX11Graphics::Instance()->GetD3DDeviceContext()->RSSetState(0);
			TheDX11Graphics::Instance()->GetD3DDeviceContext()->OMSetDepthStencilState(0,0);
			oldCam->Use();
		}

	}
	bool DX11DeferredPointLightSet::AddLight(PointLight& light)
	{

		activeDir_.push_back(light);
		return true;
	}

	void DX11DeferredPointLightSet::PushLight(PointLight& light)
	{


		activeDir_.push_back(light);
	}


	void DX11DeferredPointLightSet::SetAttenuationAll(Vec3f att)
	{
		for (int i = 0; i < activeDir_.size(); ++i)
		{
			activeDir_[i].SetAttenuation(att);
		}

	}


	void DX11DeferredPointLightSet::Clear()
	{
		activeDir_.clear();
	}