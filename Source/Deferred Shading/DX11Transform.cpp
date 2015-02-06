#include "DX11Transform.h"

//for setting the current states matricies
//---
#include "DX11Graphics.h"
//--
DX11Transform::DX11Transform()
{
scale_=Vec3f(1,1,1);
}

DX11Transform::~DX11Transform()
{
}


void DX11Transform::Stop()
{
	TheDX11Graphics::Instance()->GetCurrentDrawState()->SetWorld(reset_);
	TheDX11Graphics::Instance()->GetCurrentDrawState()->BindWorld();
}

void DX11Transform::Reset()
{
scale_=Vec3f(1,1,1);
move_=Vec3f(0,0,0);
rot_=Vec3f(0,0,0);
reset_.SetToIdentity();
}
void DX11Transform::AddUse()
{
	reset_=TheDX11Graphics::Instance()->GetCurrentDrawState()->GetWorld();
	Matrix m;
	m.Scale(scale_);
	m.MulRotateX(rot_.x);
	m.MulRotateY(rot_.y);
	m.MulRotateZ(rot_.z);
	m.MulTranslate(move_);
	TheDX11Graphics::Instance()->GetCurrentDrawState()->MulWorld(m);
	TheDX11Graphics::Instance()->GetCurrentDrawState()->BindWorld();
}

void DX11Transform::Use()
{
	reset_=TheDX11Graphics::Instance()->GetCurrentDrawState()->GetWorld();
	Matrix m;
	m.Scale(scale_);
	m.MulRotateX(rot_.x);
	m.MulRotateY(rot_.y);
	m.MulRotateZ(rot_.z);
	m.MulTranslate(move_);
	TheDX11Graphics::Instance()->GetCurrentDrawState()->SetWorld(m);
	TheDX11Graphics::Instance()->GetCurrentDrawState()->BindWorld();
}

void DX11Transform::Translate(Vec3f trans)
{
	move_=trans;
}

void DX11Transform::AddTranslate(Vec3f trans)
{
	move_+=trans;
}

void DX11Transform::RotateX(float rad)
{
rot_.x=rad;
}

void DX11Transform::RotateY(float rad)
{
rot_.y=rad;
}

void DX11Transform::RotateZ(float rad)
{
rot_.z=rad;
}

void DX11Transform::Scale(Vec3f scale)
{
scale_*=scale;
}