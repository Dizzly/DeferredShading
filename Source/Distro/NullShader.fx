

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

struct PSInput{float4 pos:SV_POSITION;};


PSInput VS( VSInput input)
{
PSInput output=(PSInput)0;
output.pos= mul(input.pos,c_world);
output.pos=mul(output.pos,c_view);
output.pos=mul(output.pos,c_proj);

return output;
}

void PS(PSInput input)
{

}