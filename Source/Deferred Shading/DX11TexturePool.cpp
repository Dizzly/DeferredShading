#include "DX11TexturePool.h"

bool DX11TexturePool::LoadPoolObject(const std::string& filename, const std::string& objectname)
{
	if(objMap_[objectname].Load(filename))
	{
		return true;
	}
	return false;

}

DX11Texture* DX11TexturePool::GetPoolObject(const std::string& str)
{
	TextureMap::iterator it=objMap_.find(str);
	if(it!=objMap_.end())
	{return &(*it).second;}
	return NULL;
}