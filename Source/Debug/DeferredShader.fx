
Texture2D colorTex:register(t0);


SamplerState SamplePoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

cbuffer WorldMaatrix :register (b11)
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


struct VSInput{
	float4 pos:POSITION;
	float4 normal:NORMAL;
	float2 tex:TEXCOORD0;
};

struct PSInput{
	float4 pos:SV_POSITION;
	float4 vPos:POSITION;
	float4 normal:TEXCOORD0;
	float2 tex:TEXCOORD1;
};

struct PSOutput{
	float4 col:SV_Target0;
	float4 normal:SV_Target1;
	float4 tex:SV_Target2;
};


PSInput VS(VSInput input)
{
	PSInput output = (PSInput)0;
	output.pos = mul(input.pos, c_world);
	output.pos = mul(output.pos, c_view);
	output.vPos = output.pos;
	output.pos = mul(output.pos, c_proj);

	output.normal = mul(input.normal, c_inverse);
	output.tex = input.tex;


	return output;
}

PSOutput PS(PSInput input) :SV_Target
{
	PSOutput output = (PSOutput)0;
	output.col = colorTex.Sample(SamplePoint, input.tex);
	output.col.w = 0.001f;
	output.normal.xyz = normalize(input.normal.xyz);
	output.normal.w = input.vPos.z;
	output.tex = float4(input.tex, 0, 0);
	return output;
}