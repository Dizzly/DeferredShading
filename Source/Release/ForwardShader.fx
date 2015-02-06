#include "LightHelper.fx"

Texture2D texMap:register(t0);


SamplerState SamplePoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

cbuffer WorldMatrix :register (b11)
{
	matrix c_world;
	matrix c_inverse;
}

cbuffer ViewMatrix :register (b12)
{
	matrix c_view;
	float4 c_eyePos;
}

cbuffer ProjMatrix :register (b13)
{
	matrix c_proj;
}

cbuffer Lights : register (b10)
{
	int c_dirNum;
	int c_pointNum;
	int c_spotNum;
	float NA_LightPadding; //padding for Vec4 boundry
	DirectionalLight c_dirLights[16];
	PointLight c_pointLights[540];
	SpotLight c_spotLights[16];
}



struct VSInput {
	float4 Pos: POSITION;
	float4 Normal: NORMAL;
	float2 Tex: TEXCOORD0;
};

struct PSInput {
	float4 Pos: SV_POSITION;
	float4 WPos: POSITION;
	float3 Normal: NORMAL;
	float2 Tex: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSInput VS(VSInput input)
{
	PSInput output = (PSInput)0;
	output.Pos = mul(input.Pos, c_world);
	output.WPos = output.Pos;
	output.Pos = mul(output.Pos, c_view);
	output.Pos = mul(output.Pos, c_proj);
	output.Tex = input.Tex;
	output.Normal = mul(input.Normal, c_inverse);

	return output;
}




//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PSInput input) : SV_Target
{
	Material m;
	float3 toEye = normalize(c_eyePos - input.WPos.xyz);

		float3 norm = normalize(input.Normal);
		float4 totalAmbi = float4(0, 0, 0, 0);
		float4 totalDiff = float4(0, 0, 0, 0);
		float4 totalSpec = float4(0, 0, 0, 0);

		float4 retAmbient;
	float4 retDiffuse;
	float4 retSpec;

	float4 col = texMap.Sample(SamplePoint, input.Tex);
		m.Diffuse = col;
	m.Ambient = col;
	m.Specular = float4(1, 1, 1, 0.001f);
	for (int i = 0; i < c_dirNum; ++i)
	{
		ComputeDirLight(m, c_dirLights[i],
			norm, toEye,
			retAmbient,
			retDiffuse,
			retSpec);

		totalAmbi += retAmbient;
		totalDiff += retDiffuse;
		totalSpec += retSpec;
	}
	for (int j = 0; j < c_pointNum; ++j)
	{
		ComputePointLight(m, c_pointLights[j],
			input.WPos, norm, toEye,
			retAmbient,
			retDiffuse,
			retSpec);

		totalAmbi += retAmbient;
		totalDiff += retDiffuse;
		totalSpec += retSpec;
	}
	float4 r = totalAmbi + totalDiff +totalSpec;
		return r;
}