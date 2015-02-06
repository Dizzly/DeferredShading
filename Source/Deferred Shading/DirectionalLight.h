#ifndef DIRECTIONAL_LIGHT_H_INCLUDED
#define DIRECTIONAL_LIGHT_H_INCLUDED


//memory managment for small objects
//---
#include "SmartPointer.h"
//--

//inherited variables
//---
#include "Light.h"
//

//data structs
//---
#include "Vec4.h"
#include "Vec3.h"
//--

class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(Vec4f ambi, Vec4f diff, Vec4f spec,
		Vec3f dir);
	~DirectionalLight();
	void SetDir(Vec3f dir);
	void SetAmbient(Vec4f amb);
	void SetDiffuse(Vec4f diff);
	void SetSpecular(Vec4f spec);
private:
	Vec4f ambient_;
	Vec4f diffuse_;
	Vec4f specular_;
	Vec3f dir_;
	float padding_;
};
#endif