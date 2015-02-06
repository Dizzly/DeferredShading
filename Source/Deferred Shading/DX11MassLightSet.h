#pragma once

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

template < class TDirLight = DirectionalLight,
class TPointLight = PointLight,
class TSpotLight = SpotLight
>

class DX11MassLightSet
{



};