#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

//data structs
//---
#include "Vec4.h"
//--

//smart pointer support
//---
#include "SmartPointer.h"
//--

class Light :public RefCounted
{
public:
	virtual ~Light();
	void SetAmbient(Vec4f amb);
	void SetDiffuse(Vec4f diff);
	void SetSpecular(Vec4f spec);
protected:
	Vec4f ambient_;
	Vec4f diffuse_;
	Vec4f specular_;
};

#endif