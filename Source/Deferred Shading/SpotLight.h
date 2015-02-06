#ifndef SPOT_LIGHT_H_INCLUDED
#define SPOT_LIGHT_H_INCLUDED

//memory managment for small objects
//---
#include "SmartPointer.h"
//--

//base class
//---
#include "Light.h"
//--

//data structs
//---
#include "Vec3.h"
//--

class SpotLight :public Light
{
public:
	SpotLight();
	SpotLight(Vec4f ambi, Vec4f diff, Vec4f spec,
		Vec3f pos, float range, Vec3f dir, float spotDeg);
	~SpotLight();

	void SetPos(Vec3f pos);
	void SetDir(Vec3f dir);
	void SetRange(float range);
	void SetSpot(float spot);

	Vec3f GetPos()const;
	Vec3f GetDir()const;
	float GetRange()const;
	float GetSpot()const;

	void LookAt(Vec3f point);

	void SetAmbient(Vec4f amb);
	void SetDiffuse(Vec4f diff);
	void SetSpecular(Vec4f spec);
private:
	Vec4f ambient_;
	Vec4f diffuse_;
	Vec4f specular_;

	Vec3f pos_;
	float range_;

	Vec3f dir_;
	float spot_;
	
	Vec3f attenuation_;
	float padding_;
};
typedef SmartPointer<SpotLight> PSpotLight;
#endif