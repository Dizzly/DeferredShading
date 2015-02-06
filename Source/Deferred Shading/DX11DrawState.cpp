#include "DX11DrawState.h"

void DX11DrawState::SetWorld(XMMATRIX& mat){world_=mat;}
	void DX11DrawState::SetView(XMMATRIX& mat){view_=mat;}
	void DX11DrawState::SetProj(XMMATRIX& mat){proj_=mat;}

	void DX11DrawState:: SetWorld(Matrix& mat){world_=mat.GetMatrix();}
	void DX11DrawState::SetView(Matrix& mat){view_=mat.GetMatrix();}
	void DX11DrawState::SetProj(Matrix& mat){proj_=mat.GetMatrix();}

	void DX11DrawState::MulWorld(XMMATRIX& mat){world_=XMMatrixMultiply(world_,mat);}
	void DX11DrawState::MulView(XMMATRIX& mat){view_=XMMatrixMultiply(view_,mat);}

	void DX11DrawState::MulWorld(Matrix& mat){
	mat.Multiply(&world_._11);
	world_=mat.GetMatrix();
	}
	void DX11DrawState::MulView(Matrix& mat){
	mat.Multiply(&view_._11);
	view_=mat.GetMatrix();}

	XMMATRIX DX11DrawState::GetWorldDX()const{return world_;}
	XMMATRIX DX11DrawState::GetViewDX()const{return view_;}
	XMMATRIX DX11DrawState::GetProjDX()const{return proj_;}

	Matrix DX11DrawState::GetWorld()const{return Matrix(&world_._11);}
	Matrix DX11DrawState::GetView()const{return Matrix(&view_._11);}
	Matrix DX11DrawState::GetProj()const{return Matrix(&proj_._11);}

	void DX11DrawState::SetCam(DX11Camera* cam){baseCam_=cam;}
	DX11Camera* DX11DrawState::GetCam(){return baseCam_;}