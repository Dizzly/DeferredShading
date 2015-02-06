
Texture2D colorTex:register(t1);
Texture2D normalTex:register(t2);
Texture2D texTex:register(t3);


SamplerState SamplePoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU=Clamp;
	AddressV=Clamp;
};

cbuffer LightBuf :register (b2)
{
	float4 c_ambient;
	float4 c_diffuse;
	float4 c_specular;
	float3 c_pos;
	float c_range;
	float3 c_att;
	float c_pad;
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
				float3 wPos:POSITION;
				float3 viewRay:POSITION1;};


PSInput VS( VSInput input)
{
PSInput output=(PSInput)0;
output.pos= mul(input.pos,c_world);
output.wPos = output.pos;
output.pos=mul(output.pos,c_view);

output.pos=mul(output.pos,c_proj);

output.viewRay=output.wPos-c_eyePos;
return output;
}

float4 PS(PSInput input):SV_Target
{
float2 tex=float2(input.pos.x/750.0f,input.pos.y/750.0f);
float4 colSamp=colorTex.Sample(SamplePoint,tex);
float4 matDiffuse=float4(colSamp.xyz,0);
float matSpec=colSamp.w;
float4 normSamp=normalTex.Sample(SamplePoint,tex);
float3 normal=normSamp.xyz;
float depth=normSamp.w;


float3 viewRay = normalize(input.viewRay);

float3 worldPos = c_eyePos + viewRay *depth;

float3 toEye=c_eyePos.xyz-worldPos;
float3 lightVec=c_pos-worldPos;
float d=length(lightVec);



lightVec/=d;
float4 ambient = float4(colSamp.xyz,0)*c_ambient;
float4 diffuse=float4(0,0,0,0);
float4 spec=float4(0,0,0,0);
float dF= dot(normal,lightVec);


if(dF>0.0f)
{

	float3 v=reflect(lightVec,normal);
	float specFactor = pow(max(dot(v,toEye),0.0f),matSpec);
	diffuse = dF* matDiffuse * c_diffuse;
	spec= specFactor * c_specular;
}
float att = 1.0f/dot(c_att, float3(1.0f,d,d*d));

diffuse*=att;
spec*=att;
return ambient + diffuse +spec;
}

