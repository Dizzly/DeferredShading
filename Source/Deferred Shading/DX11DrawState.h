#ifndef DX11_DRAW_STATE_H_INCLUDED
#define DX11_DRAW_STATE_H_INCLUDED

//base class
//---
#include "State.h"
//---

//maths lib for dx
//---
#include <D3D11.h>
//XNAMATH requires D3D11!!!!!!!
#include <xnamath.h>
//---

//my own matrix class
//---
#include "Matrix.h"
//--


//class predif, with smart pointer define
//---
#include "SmartPointer.h"
class DX11Camera;
//--
//classes inheriting should bind the world buffers appropriately
class DX11DrawState :public State
{
public://inhereted members
	virtual ~DX11DrawState()=0{}
	virtual void Update()=0{}
	virtual void Draw()=0{}
	virtual void OnActive()=0{}
	virtual void OnInactive()=0{}
public://new virtual members
	virtual void BindWorld()=0{};
	virtual void BindView()=0{};
	virtual void BindProj()=0{};

	virtual void ClearResources()=0{};
public://new members
	void SetWorld(XMMATRIX& mat);
	void SetView(XMMATRIX& mat);
	void SetProj(XMMATRIX& mat);

	void SetWorld(Matrix& mat);
	void SetView(Matrix& mat);
	void SetProj(Matrix& mat);

	void MulWorld(XMMATRIX& mat);
	void MulView(XMMATRIX& mat);

	void MulWorld(Matrix& mat);
	void MulView(Matrix& mat);

	XMMATRIX GetWorldDX()const;
	XMMATRIX GetViewDX()const;
	XMMATRIX GetProjDX()const;

	Matrix GetWorld()const;
	Matrix GetView()const;
	Matrix GetProj()const;
public: //camera funcs
	void SetCam(DX11Camera* cam);
	DX11Camera* GetCam();
protected:
	DX11Camera* baseCam_;
	XMMATRIX world_,
		view_,
		proj_;
};

#endif