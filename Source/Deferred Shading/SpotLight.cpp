#include "SpotLight.h"

SpotLight::SpotLight()
{range_=0.0f;spot_=0.0f;}

SpotLight::SpotLight(Vec4f ambi, Vec4f diff, Vec4f spec,
	Vec3f pos, float range, Vec3f dir, float spotDeg):
pos_(pos),range_(range), dir_(dir), spot_(spotDeg)
{
	ambient_=ambi;
	diffuse_=diff;
	specular_=spec;
}
SpotLight::~SpotLight()
{}

void SpotLight::LookAt(Vec3f point)
{
	dir_=(point-pos_);
	dir_.Normalise();
}

void SpotLight::SetPos(Vec3f pos){pos_=pos;}
void SpotLight::SetDir(Vec3f dir){dir_=dir;}
void SpotLight::SetRange(float range){range_=range;}
void SpotLight::SetSpot(float spot){spot_=spot;}

Vec3f SpotLight::GetPos()const{return pos_;}
Vec3f SpotLight::GetDir()const{return dir_;}
float SpotLight::GetRange()const{return range_;}
float SpotLight::GetSpot()const{return spot_;}

void SpotLight::SetAmbient(Vec4f amb)
{ambient_=amb;}

void SpotLight::SetDiffuse(Vec4f diff)
{diffuse_=diff;}

void SpotLight::SetSpecular(Vec4f spec)
{specular_=spec;}