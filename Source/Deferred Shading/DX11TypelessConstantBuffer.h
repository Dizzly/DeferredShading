#pragma once
#include <D3DX11.h>
#include <Windows.h>
#include <assert.h>
#include "DX11Graphics.h"

//similar to ConstantBuffer, but does not require a type as a template argument
//simply creates a constant buffer of a specififed size on creation
//no error checking for update function as to contingent sizes

class DX11TypelessConstantBuffer
{
public:
	enum BUFFER_USAGE {BUFFER_DEAFULT=0,BUFFER_DYNAMIC=2};
public:
	//----------
	DX11TypelessConstantBuffer();
	//----------
	~DX11TypelessConstantBuffer();
	//----------
	bool Init(int id, int size, BUFFER_USAGE state=BUFFER_DEAFULT);
	//----------
	void Update(void* t);
	//----------
	void Bind(int id=-1);
public:

private:
	ID3D11Buffer* pConstantBuffer_;
	BUFFER_USAGE state_;
	int id_;
};