#ifndef DX11_TEXTURE_POOL_H_INCLUDED
#define DX11_TEXTURE_POOL_H_INCLUDED


//rest of texture stuff, pool is non copyable
//---
#include "Singleton.h"
//
//texture for pooling
//---
#include "DX11Texture.h"
#include <map>
//--
//the object pool
//---
#include "Pool.h"
//--
class DX11TexturePool;
typedef Singleton<DX11TexturePool> TheDX11TexturePool;

class DX11TexturePool :public Pool<DX11Texture>
{
public:
	typedef std::map<std::string,DX11Texture> TextureMap;
	bool LoadPoolObject(const std::string& filename, const std::string& objectname)override;
	DX11Texture* GetPoolObject(const std::string& str);
private:
	DX11TexturePool(){};
	friend TheDX11TexturePool;
	private:
	TextureMap objMap_;
};



#endif