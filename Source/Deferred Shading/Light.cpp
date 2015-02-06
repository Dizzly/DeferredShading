#include "Light.h"

Light::~Light()
{}
void Light::SetAmbient(Vec4f amb)
{ambient_=amb;}
void Light::SetDiffuse(Vec4f dif)
{diffuse_=dif;}
void Light::SetSpecular(Vec4f spec)
{specular_=spec;}