#ifndef DX11_TEX_SAMPLER_H_INCLUDED
#define DX11_TEX_SAMPLER_H_INCLUDED

//direct x
//---
#include <D3D11.h>
//--

class DX11TexSampler
{
public:
	DX11TexSampler();
	~DX11TexSampler();
	enum SAMPLE_METHOD{SAMPLE_LINEAR=0};
	bool Init(int sampleMethod=0,int id=0);
	void Use(int id=0);
private:
	int id_;
	ID3D11SamplerState* pSampler_;

};
#endif