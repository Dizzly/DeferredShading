#include "DX11Camera.h"

DX11Camera::DX11Camera()
{
	ortho_=false;
}
DX11Camera::~DX11Camera()
{

}
void DX11Camera::Use()
{
	TheDX11Graphics::Instance()->GetCurrentDrawState()->SetCam(this);
	XMMATRIX mat;
	XMVECTOR eye =XMVectorSet(pos_.x,pos_.y,pos_.z,1.0f);
	XMVECTOR at = XMVectorSet(lookAt_.x,lookAt_.y,lookAt_.z,1.0f);
	XMVECTOR up = XMVectorSet(up_.x,up_.y,up_.z,1.0f);
	mat = XMMatrixLookAtLH(eye,at,up);
	TheDX11Graphics::Instance()->GetCurrentDrawState()->SetView(mat);
	if(lens_!=Vec4f())
	{
		XMMATRIX proj;
		if(!ortho_)
		{
		proj=XMMatrixPerspectiveFovLH(lens_.x,lens_.y,lens_.z,lens_.w);
		}
		else
		{
		proj=XMMatrixOrthographicLH(lens_.x,lens_.y,lens_.z,lens_.w);
		}
		TheDX11Graphics::Instance()->GetCurrentDrawState()->SetProj(proj);
		TheDX11Graphics::Instance()->GetCurrentDrawState()->BindProj();
	}
	TheDX11Graphics::Instance()->GetCurrentDrawState()->BindView();
}

void DX11Camera::SetPos(Vec3f pos){pos_=pos;}
void DX11Camera::Move(Vec3f moveVec){pos_+=moveVec;}
void DX11Camera::LookAt(Vec3f point){lookAt_=point;}
void DX11Camera::SetLens(Vec4f lens){lens_=lens;}

void DX11Camera::SetUp(Vec3f up){up_=up;}

Vec3f DX11Camera::GetPos()const{return pos_;}
Vec3f DX11Camera::GetLookedAt()const{return lookAt_;}
Vec3f DX11Camera::GetLookVec()const{return lookAt_-pos_;}
Vec3f DX11Camera::GetUp()const{return up_;}

Matrix DX11Camera::CalcView()const
{
	XMMATRIX mat;
	XMVECTOR eye =XMVectorSet(pos_.x,pos_.y,pos_.z,1.0f);
	XMVECTOR at = XMVectorSet(lookAt_.x,lookAt_.y,lookAt_.z,1.0f);
	XMVECTOR up = XMVectorSet(up_.x,up_.y,up_.z,1.0f);
	mat = XMMatrixLookAtLH(eye,at,up);
	return Matrix(&mat._11);
}
Matrix DX11Camera::CalcViewProj()const
{
	XMMATRIX proj;
	if(lens_!=Vec4f())
	{
		if(!ortho_)
		{
			proj=XMMatrixPerspectiveFovLH(lens_.x,lens_.y,lens_.z,lens_.w);
		}
		else
		{
			proj=XMMatrixOrthographicLH(lens_.x,lens_.y,lens_.z,lens_.w);
		}
	}
	else
	{
		proj= TheDX11Graphics::Instance()->GetCurrentDrawState()->GetProjDX();
	}
	XMMATRIX view;
	XMVECTOR eye =XMVectorSet(pos_.x,pos_.y,pos_.z,1.0f);
	XMVECTOR at = XMVectorSet(lookAt_.x,lookAt_.y,lookAt_.z,1.0f);
	XMVECTOR up = XMVectorSet(up_.x,up_.y,up_.z,1.0f);
	view = XMMatrixLookAtLH(eye,at,up);
	XMMATRIX viewProj= view*proj;
	return Matrix(&viewProj._11);
}


void DX11Camera::SetOrtho(bool b)
{
	ortho_=b;
}