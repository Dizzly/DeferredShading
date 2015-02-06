#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{}

DirectionalLight::DirectionalLight(Vec4f ambi, Vec4f diff, Vec4f spec,
		Vec3f dir):dir_(dir)
{
	ambient_=ambi;
	diffuse_=diff;
	specular_=spec;
}

DirectionalLight::~DirectionalLight()
{}
void DirectionalLight::SetDir(Vec3f dir)
{dir_=dir;}

void DirectionalLight::SetAmbient(Vec4f amb)
{ambient_=amb;}
	
void DirectionalLight::SetDiffuse(Vec4f diff)
	{diffuse_=diff;}
	
void DirectionalLight::SetSpecular(Vec4f spec)
	{specular_=spec;}