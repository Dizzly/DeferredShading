#ifndef DX11_TRANSFORM_H_INCLUDED
#define DX11_TRANSFORM_H_INCLUDED

//data structs
//---
#include "Vec3.h"
#include "Matrix.h"
//--
class DX11Transform
{
public:
	DX11Transform();
	~DX11Transform();
	
	void AddUse();
	void Use();

	void Stop();
	void Reset();

	void Translate(Vec3f trans);
	void AddTranslate(Vec3f trans);
	void RotateX(float rad);
	void RotateY(float rad);
	void RotateZ(float rad);
	void Scale(Vec3f scale);
private:
	Matrix reset_;
	Vec3f move_;
	Vec3f rot_;
	Vec3f scale_;
};

#endif