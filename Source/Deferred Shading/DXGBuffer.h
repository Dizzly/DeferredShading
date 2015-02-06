#pragma once
#include <D3DX11.h>

const int BUFF_SIZE=4;
class DXGBuffer
{
public:
	DXGBuffer();
	~DXGBuffer();

	bool Init();
	void Shutdown();
	void BindToWrite();
	void BindToRead(int startingID=0);
	ID3D11Texture2D* GetTex(int i);
	void Clear();

private:
	ID3D11Texture2D* texes_[BUFF_SIZE];
	ID3D11RenderTargetView* renderTarget_[BUFF_SIZE];
	ID3D11ShaderResourceView* texViews_[BUFF_SIZE];
	D3D11_VIEWPORT viewport_;
};