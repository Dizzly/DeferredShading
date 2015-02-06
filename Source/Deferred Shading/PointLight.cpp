#include "PointLight.h"

PointLight::PointLight()
{range_=0.0f;}

PointLight::PointLight(Vec4f ambi, Vec4f diff, Vec4f spec,
	Vec3f pos, float range, Vec3f attent):
pos_(pos), range_(range), attenuation_(attent)
{
	ambient_=ambi;
	diffuse_=diff;
	specular_=spec;
}
PointLight::~PointLight()
{}

void PointLight::MoveBy(Vec3f vec){pos_+=vec;}

void PointLight::SetPos(Vec3f pos){pos_=pos;};
void PointLight::SetRange(float range){range_=range;}
void PointLight::SetAttenuation(Vec3f att){attenuation_=att;};
Vec3f PointLight::GetAttenuation()const{return attenuation_;}

float PointLight::GetRange()const{return range_;}
Vec3f PointLight::GetPos()const{return pos_;}

void PointLight::SetAmbient(Vec4f amb)
{ambient_=amb;}

void PointLight::SetDiffuse(Vec4f diff)
{diffuse_=diff;}

void PointLight::SetSpecular(Vec4f spec)
{specular_=spec;}

Vec4f PointLight::GetDiffuse()const
{return diffuse_;}

Vec4f PointLight::GetAmbient()const
{return ambient_;}

Vec4f PointLight::GetSpecular()const
{return specular_;}