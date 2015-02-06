#ifndef DX11TEXTURE_H_INCLUDED
#define DX11TEXTURE_H_INCLUDED
#include <string>
#include "SmartPointer.h"
#include <D3DX11.h>

class DX11Texture :public RefCounted
{
public:
	DX11Texture();
	~DX11Texture();
	bool Load(const std::string& filename,int ID=0);
	void SetID(int id);
	int GetID()const;
	void Use(int id=0);
	void Stop();
private:
	ID3D11ShaderResourceView* pTexture_;
	int id_;
};

#endif