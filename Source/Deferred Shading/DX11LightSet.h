#ifndef DX11_LIGHT_MANAGER_H_INCLUDED
#define DX11_LIGHT_MANAGER_H_INCLUDED


//light classes
//---
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
//--

//error checking
//---
#include <assert.h>
//--


// data structures
//---
#include <map>
#include <vector>
//--

// direct x management
//---
#include "DX11ConstantBuffer.h"
//--

//memory management
//---
#include "SmartPointer.h"
//--



template <int maxLights,//the max lights must be a multiple of 16
	int numDirLights = maxLights,
	int numPointLights = maxLights,
	int numSpotLights = maxLights,
class TDirLight = DirectionalLight,
class TPointLight = PointLight,
class TSpotLight = SpotLight
>
class DX11LightSet
{
private:
	struct LightBuffer;

	typedef std::vector<TDirLight> ActiveDir;
	typedef std::vector<TSpotLight> ActiveSpot;
	typedef std::vector<TPointLight> ActivePoint;
public:
	void Init(int bindSlot)
	{
		assert(maxLights != 0);
		size_ = (sizeof(TDirLight)*numDirLights + sizeof(TPointLight)*numPointLights +
			sizeof(TSpotLight)*numSpotLights) / sizeof(float);
		lightBuffer_.Init(bindSlot);
	}

	void Use()
	{
		int i = 0;
		lightBuffer_.Bind();
		LightBuffer b;
		b.dirNum = activeDir_.size();
		for (i = 0; i < b.dirNum; ++i)
		{
			b.dirLights[i] = activeDir_[i];
		}
		b.pointNum = activePoint_.size();
		for (i = 0; i < b.pointNum; ++i)
		{
			b.pointLights[i] = activePoint_[i];
		}
		b.spotNum = activeSpot_.size();
		for (i = 0; i < b.spotNum; ++i)
		{
			b.spotLights[i] = activeSpot_[i];
		}
		lightBuffer_.Update(b);
	}
	//add functions return false if the buffer is full of lights
	bool AddDirLight(TDirLight&  light)
	{
		if (activeDir_.size() >= (unsigned int)numDirLights)
		{
			return false;
		}
		activeDir_.push_back(light);
		return true;
	}//
	bool AddSpotLight(TSpotLight& light)
	{
		if (activeSpot_.size() >= (unsigned int)numSpotLights)
		{
			return false;
		}
		activeSpot_.push_back(light);
		return true;
	}
	bool AddPointLight(TPointLight& light)
	{
		if (activePoint_.size() >= (unsigned int)numPointLights)
		{
			return false;
		}
		activePoint_.push_back(light);
		return true;
	}
	//end of add funcs
	//push funcs will always add the light, will swap out the first light added
	void PushDirLight(TDirLight&  light)
	{
		if (activeDir_.size() >= (unsigned int)numDirLights)
		{
			std::swap(activeDir_.front(), activeDir_.back());
			activeDir_.pop_back();
		}
		activeDir_.push_back(light);
	}
	void PushSpotLight(TSpotLight& light)
	{
		if (activeSpot_.size() >= (unsigned int)numSpotLights)
		{
			std::swap(activeSpot_.front(), activeSpot_.back());
			activeSpot_.pop_back();
		}
		activeSpot_.push_back(light);
	}
	void PushPointLight(TPointLight& light)
	{
		if (activePoint_.size() >= (unsigned int)numPointlights)
		{
			std::swap(activePoint_.front(), activePoint_.back());
			activePoint_.pop_back();
		}
		activePoint_.push_back(light);
	}
	//end of push
	//clear erases that vectors contents
	void ClearDirLights()
	{
		activeDir_.clear();
	}
	void ClearSpotLights()
	{
		activeSpot_.clear();
	}
	void ClearPointLights()
	{
		activePoint_.clear();
	}
	//end of clear
	//remove removes a single element through swapping
	void RemoveDirLight(TDirLight& light)
	{
		for (auto it = activeDir_.begin(); it != activeDir_.end(); ++it)
		{
			if ((*it) == light)
			{
				std::swap((*it), activeDir_.back());
				activeDir_.pop_back();
			}
		}
	}
	void RemovePointLight(TPointLight& light)
	{
		for (auto it = activePoint_.begin(); it != activePoint_.end(); ++it)
		{
			if ((*it) == light)
			{
				std::swap((*it), activePoint_.back());
				activePoint_.pop_back();
			}
		}
	}
	void RemoveSpotLight(TSpotLight& light)
	{
		for (auto it = activeSpot_.begin(); it != activeSpot_.end(); ++it)
		{
			if ((*it) == light)
			{
				std::swap((*it), activeSpot_.back());
				activeSpot_.pop_back();
			}
		}
	}
	//end of remove
private:
	int bindSlot_;
	int size_;

	DX11ConstantBuffer<LightBuffer> lightBuffer_;
	ActiveDir activeDir_;
	ActiveSpot activeSpot_;
	ActivePoint activePoint_;
private:
	struct LightBuffer
	{
		LightBuffer()
		{
			dirNum = 0; pointNum = 0; spotNum = 0;
			padding = 0;
		}
		int dirNum, pointNum, spotNum;
		float padding;
		TDirLight dirLights[numDirLights + (16 - (numDirLights % 16 == 0 ? 16 : numDirLights % 16))];
		TPointLight pointLights[numPointLights + (16 - (numPointLights % 16 == 0 ? 16 : numPointLights % 16))];
		TSpotLight spotLights[numSpotLights + (16 - (numSpotLights % 16 == 0 ? 16 : numSpotLights % 16))];
	};
};
#endif