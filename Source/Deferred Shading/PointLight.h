#ifndef POINT_LIGHT_H_INCLUDED
#define POINT_LIGHT_H_INCLUDED

//memory managment for small objects
//---
#include "SmartPointer.h"
//--

//inherited varaibles and setters
//---
#include "Light.h"
//--

//data structs
//---
#include "Vec3.h"
//--

class PointLight
{
public:
	PointLight();
	PointLight(Vec4f ambi, Vec4f diff, Vec4f spec,
		Vec3f pos, float range, Vec3f attent);
	~PointLight();

	void SetPos(Vec3f pos);
	void MoveBy(Vec3f vec);
	void SetRange(float range);
	void SetAttenuation(Vec3f atten);
	Vec3f GetAttenuation()const;
	Vec3f GetPos()const;
	float GetRange()const;
	
	void SetAmbient(Vec4f amb);
	Vec4f GetAmbient()const;
	void SetDiffuse(Vec4f diff);
	Vec4f GetDiffuse()const;
	void SetSpecular(Vec4f spec);
	Vec4f GetSpecular()const;
private:
	Vec4f ambient_;
	Vec4f diffuse_;
	Vec4f specular_;

	Vec3f pos_;
	float range_;

	Vec3f attenuation_;
	float padding_;//padding to fit into four float boundries
};
#endif