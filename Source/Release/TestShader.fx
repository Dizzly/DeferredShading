
Texture2D colorTex:register(t1);
Texture2D normalTex:register(t2);
Texture2D texTex:register(t3);


SamplerState SamplePoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU=Clamp;
	AddressV=Clamp;
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


struct VSInput{float4 pos:POSITION;
				float2 tex:TEXCOORD0;};

struct PSInput{float4 pos:SV_POSITION;
				float2 tex:TEXCOORD0;};


PSInput VS( VSInput input)
{
PSInput output=(PSInput)0;
output.pos= mul(input.pos,c_world);
output.pos=mul(output.pos,c_view);
output.pos=mul(output.pos,c_proj);

output.tex=input.tex;
return output;
}

float4 PS(PSInput input):SV_Target
{
	float4 samp= texTex.Sample(SamplePoint,input.tex);
	return samp;
}