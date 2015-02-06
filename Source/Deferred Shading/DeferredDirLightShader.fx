
Texture2D colorTex:register(t1);
Texture2D normalTex:register(t2);
Texture2D texTex:register(t3);


SamplerState SamplePoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU=Clamp;
	AddressV=Clamp;
};

cbuffer LightBuf :register (b1)
{
	float4 c_ambient;
	float4 c_diffuse;
	float4 c_specular;
	float3 c_dir;
	float c_padding;
}


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


struct VSInput{float4 pos:POSITION;};

struct PSInput{float4 pos:SV_POSITION;
				float4 viewRay:POSITION;};


PSInput VS( VSInput input)
{
PSInput output=(PSInput)0;
output.pos= mul(input.pos,c_world);
float4 wPos=output.pos;
output.pos=mul(output.pos,c_view);

output.pos=mul(output.pos,c_proj);

output.viewRay=wPos-c_eyePos;
return output;
}

float4 PS(PSInput input):SV_Target
{
	float2 tex=float2(input.pos.x/1280,input.pos.y/648);
	float4 colSamp= colorTex.Sample(SamplePoint,tex);
	float matSpec=colSamp.w;
	float4 normSamp = normalTex.Sample(SamplePoint,tex);
	float3 normal = normSamp.xyz;
	float depth=normSamp.w;
	float3 lightVec= -c_dir;
	float4 ambient=float4(0.0f,0.0f,0.0f,0.0f);
	float4 diffuse=float4(0.0f,0.0f,0.0f,0.0f);
	float4 spec=float4(0.0f,0.0f,0.0f,0.0f);

	ambient = colSamp * c_ambient;
	float3 viewRay=normalize(input.viewRay.xyz);
	float3 worldPos=c_eyePos.xyz + viewRay*depth;

	float3 toEye=c_eyePos.xyz-worldPos;

	float dF= dot(lightVec,normal);
	if(dF>0.0f)
	{
	float3 v=reflect(-lightVec,normal);
	float specFactor = pow(max(dot(v,toEye),0.0f),c_specular.w);
	
	diffuse = dF * colSamp * c_diffuse;
	spec = specFactor *  matSpec * c_specular;
	}
	
	
	return	ambient+diffuse;
}