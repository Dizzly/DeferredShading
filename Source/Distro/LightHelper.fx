

//adapted from FD.Luna. Introduction To 3D Game Programming with Direct X11. 
struct Material
{
float4 Ambient;
float4 Diffuse;
float4 Specular;
};

struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float padding;
    //padding variable to align the struct to vec4 boundry
};
struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Att;
    float padding;
};
struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Position;
    float Range;
    float3 Direction;
    float Spot;
    float3 Att;
    float padding;
};

void ComputeDirLight(Material mat,DirectionalLight light,
float3 normal, float3 toEye,
out float4 ambient, 
out float4 diffuse, 
out float4 spec)
{
	
	ambient=float4(0.0f,0.0f,0.0f,0.0f);
	diffuse=float4(0.0f,0.0f,0.0f,0.0f);
	spec=float4(0.0f,0.0f,0.0f,0.0f);

	float3 lightVec =-light.Direction;
	ambient=mat.Ambient *light.Ambient;
	float dF = dot(normal, lightVec);

	if(dF>0.0f)
	{
	float3 v= reflect(-lightVec,normal);
	float specFactor = pow(max(dot(v,toEye),0.0f), mat.Specular.w);

	diffuse= dF* mat.Diffuse * light.Diffuse;
	spec= specFactor* mat.Specular* light.Specular;
	}
}

void ComputePointLight(Material mat, PointLight light, float3 pos,
float3 normal, float3 toEye,
out float4 ambient,
out float4 diffuse,
out float4 spec)
{
	ambient=float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse=float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec=float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 lightVec = light.Position - pos;

	float d= length(lightVec);
	if(d>light.Range)
	{
		
		return;
	}
	//normalise without sqrting again
	lightVec/=d;
	ambient= mat.Ambient * light.Ambient;
	float dF = dot(normal,lightVec);
	if(dF>0.0f)
	{
	float3 v= reflect(lightVec,normal);
	float specFactor= pow(max(dot(v,toEye),0.0f),mat.Specular.w);
	diffuse= dF * mat.Diffuse * light.Diffuse;
	spec= specFactor * mat.Specular * light.Specular;
	}
	float att= 1.0f/dot(light.Att,float3(1.0f,d,d*d));
	diffuse*=att;
	spec*=att;
}