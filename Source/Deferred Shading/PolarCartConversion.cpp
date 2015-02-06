#include "PolarCartConversion.h"
#include <math.h>
Vec3f PolarToCart(const Vec3f& pol)
{
	//legnth, verticle, horizontal
	float x=pol.x*sin(pol.y)*cos(pol.z);
	float y=pol.x*sin(pol.y)*sin(pol.z);
	float z=pol.x*cos(pol.y);
	return Vec3f(x,y,z);
}

Vec3f CartToPolar(const Vec3f& cart)
{
	float xSqr=cart.x*cart.x;
	float ySqr=cart.y*cart.y;
	float zSqr=cart.z*cart.z;

	float h=sqrt((xSqr+ySqr+zSqr));
	float r=acos(cart.x/sqrt(xSqr+ySqr)*(cart.y<0?-1:1));
	float t=acos(cart.z/h);
	return Vec3f(h,t,r);
}