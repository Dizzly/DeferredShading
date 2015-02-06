#ifndef DX11_CAMERA_H_INCLUDED
#define DX11_CAMERA_H_INCLUDED

//data structs
//---
#include "Vec3.h"
#include "Vec4.h"
#include <string>
//--

//for viewBufferBinding
//---
#include "DX11ConstantBuffer.h"
//--

//memory management of cameras
//---
#include "SmartPointer.h"
//--

//the dx11 camera is not tied to a buffer
//it will update the current states view matrix
//and set itself as the current camera 
class DX11Camera :public RefCounted
{
public:
	DX11Camera();
	~DX11Camera();
	void Use();

	void SetPos(Vec3f pos);
	void Move(Vec3f moveVec);
	void LookAt(Vec3f point);
	void SetLens(Vec4f lens);

	void SetUp(Vec3f up);

	Vec3f GetPos()const;
	Vec3f GetLookedAt()const;
	Vec3f GetLookVec()const;
	Vec3f GetUp()const;

	Matrix CalcView()const;
	Matrix CalcViewProj()const;

	void SetOrtho(bool b);
private:

	bool ortho_;
	Vec4f lens_;
	Vec3f pos_;
	Vec3f lookAt_;
	Vec3f up_;

};
typedef SmartPointer<DX11Camera> PDX11Camera;
#endif