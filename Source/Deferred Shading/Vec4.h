#ifndef VEC4_H_INCLUDED
#define VEC4_H_INCLUDED
#include "Vec4.h"
template<class T>
struct Vec4
{
	T x,y,z,w;
	Vec4(float a_x=0,float a_y=0,float a_z=0,float a_w=0): x(a_x),y(a_y),z(a_z),w(a_w)
	{}
	Vec4(const Vec4& vec)
	{
		x=vec.x;
		y=vec.y;
		z=vec.z;
		w=vec.w;
	}
	Vec4(const Vec4<T>& vec, const T& loneW)
	{
		x=vec.x;
		y=vec.y;
		z=vec.z;
		w=loneW;
	}
	Vec4 operator +(const Vec4& av)const
	{
		return Vec4(x+av.x,y+av.y,z+av.z);
	}
	Vec4 operator -(const Vec4& mv)const
	{
		return Vec4(x-mv.x,y-mv.y,z-mv.z);
	}
	Vec4 operator *(const T multi)const
	{
		return Vec4(x*multi,y*multi,z*multi);
	}
	Vec4 operator /(const T divi)const
	{
		T temp= 1/divi;
		return Vec4(x*temp,y*temp,z*temp);
	}

	Vec4 operator *(const Vec4& vec)const
	{
		return Vec4(x*vec.x,y*vec.y,z*vec.z);
	}
	Vec4 operator /(const Vec4& vec)const
	{
		return Vec4(x/vec.x,y/vec.y,z/vec.z);
	}

	void operator +=(const Vec4& addVec)
	{
		x+=addVec.x;
		y+=addVec.y;
		z+=addVec.z;
		w+=addVec.w;
	}
	void operator -=(const Vec4& minusVec)
	{
		x-=minusVec.x;
		y-=minusVec.y;
		z-=minusVec.z;
		w-=minusVec.w;
	}
	void operator *=(const Vec4& multi)
	{
		x*=multi.x;
		y*=multi.y;
		z*=multi.z;
		w*=multi.w;
	}
	void operator /=(const Vec4& divi)
	{
		x/=divi.x;
		y/=divi.y;
		z/=divi.z;
		w/=divi.w;
	}

	void operator /=(const T divi)
	{
		T temp=1/divi;
		x*=temp;
		y*=temp;
		z*=temp;
		w*=temp;
	}
	void operator *=(const T multi)
	{
		x*=multi;
		y*=multi;
		z*=multi;
		w*=multi;
	}

	Vec4 operator-()const
	{
		return Vec4(-x,-y,-z,-w);
	}

	Vec4 Cross(const Vec4& vec)const
	{
		return Vec4((y*vec.z)-(z*vec.y),
			(z*vec.x)-(x*vec.z),
			(x*vec.y)-(y*vec.x));
	}

	T Dot(const Vec4& vec)const
	{
		return x*vec.x+y*vec.y+z*vec.z;
	}

	T SqrLength()const
	{
		return (x*x+y*y+z*z)*w;
	}

	T Length()const
	{
		return sqrt(SqrLength());
	}

	void Normalise()
	{
		Vec4f vec= Me_()/Length();
		x=vec.x;
		y=vec.y;
		z=vec.z;
		w=1;
	}
	bool operator ==(const Vec4& v)const
	{
		return(x==v.x&&
			y==v.y&&
			z==v.z&&
			w==w);
	}
	bool operator !=(const Vec4& v)const
	{
		return!(x==v.x&&
			y==v.y&&
			z==v.z&&
			w==w);
	}
	T* ToC()
	{
		return &x;
	}
};
typedef Vec4<float> Vec4f;

#endif