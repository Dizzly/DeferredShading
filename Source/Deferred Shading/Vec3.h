#ifndef VEC3_H_INCLUDED
#define VEC3_H_INCLUDED
#include <math.h>
template <class T>
struct Vec3
{
	T x,y,z;
	Vec3(){x=0;y=0;z=0;};
	Vec3(T aX, T aY, T aZ){x=aX;y=aY;z=aZ;};
	Vec3 Me_()const
	{
		return Vec3(x,y,z);
	}
	Vec3(const Vec3& vec)
	{
	x=vec.x;
	y=vec.y;
	z=vec.z;
	}
	Vec3 operator +(const Vec3& av)const
	{
		return Vec3(x+av.x,y+av.y,z+av.z);
	}
	Vec3 operator -(const Vec3& mv)const
	{
		return Vec3(x-mv.x,y-mv.y,z-mv.z);
	}
	Vec3 operator *(const T multi)const
	{
		return Vec3(x*multi,y*multi,z*multi);
	}
	Vec3 operator /(const T divi)const
	{
		T temp= 1/divi;
		return Vec3(x*temp,y*temp,z*temp);
	}

	Vec3 operator *(const Vec3& vec)const
	{
		return Vec3(x*vec.x,y*vec.y,z*vec.z);
	}
	Vec3 operator /(const Vec3& vec)const
	{
		return Vec3(x/vec.x,y/vec.y,z/vec.z);
	}

	void operator +=(const Vec3& addVec)
	{
		x+=addVec.x;
		y+=addVec.y;
		z+=addVec.z;
	}
	void operator -=(const Vec3& minusVec)
	{
		x-=minusVec.x;
		y-=minusVec.y;
		z-=minusVec.z;
	}
	void operator *=(const Vec3& multi)
	{
		x*=multi.x;
		y*=multi.y;
		z*=multi.z;
	}
	void operator /=(const Vec3& divi)
	{
		x/=divi.x;
		y/=divi.y;
		z/=divi.z;
	}

	void operator /=(const T divi)
	{
		T temp=1/divi;
		x*=temp;
		y*=temp;
		z*=temp;
	}
	void operator *=(const T multi)
	{
		x*=multi;
		y*=multi;
		z*=multi;
	}

	Vec3 operator-()const
	{
		return Vec3(-x,-y,-z);
	}

	Vec3 Cross(const Vec3& vec)const
	{
		return Vec3((y*vec.z)-(z*vec.y),
			(z*vec.x)-(x*vec.z),
			(x*vec.y)-(y*vec.x));
	}

	T Dot(const Vec3& vec)const
	{
		return x*vec.x+y*vec.y+z*vec.z;
	}

	T SqrLength()const
	{
		return x*x+y*y+z*z;
	}

	T Length()const
	{
		return sqrt(SqrLength());
	}

	void Normalise()
	{
		Vec3f vec= Me_()/Length();
		x=vec.x;
		y=vec.y;
		z=vec.z;
	}
	bool operator ==(const Vec3& v)const
	{
		return(x==v.x&&
			y==v.y&&
			z==v.z);
	}
	bool operator !=(const Vec3& v)const
	{
		return!(x==v.x&&
			y==v.y&&
			z==v.z);
	}
	T* ToC()
	{
		return &x;
	}

};
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<Vec3f> Vec3f2;


#endif