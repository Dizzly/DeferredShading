#ifndef VEC2_H
#define VEC2_H

template <class T>
struct Vec2
{
    T x;
    T y;
    Vec2(){x=0;y=0;};
    Vec2(T aX,T aY){x=aX;y=aY;};
    Vec2 operator +(const Vec2& addVec)const
    {
        return Vec2(x+addVec.x,y+addVec.y);
    }
    Vec2 operator -(const Vec2& minusVec)const
    {
        return Vec2(x-minusVec.x,y-minusVec.y);
    }
    Vec2 operator *(const T multi)const
    {
        return Vec2 (x*multi,y*multi);
    }
    Vec2 operator /(const T divi)const
    {
        return Vec2(x/divi,y/divi);
    }

    void operator +=(const Vec2& addVec)
    {
        x+=addVec.x;
        y+=addVec.y;
    }
    void operator -=(const Vec2& minusVec)
    {
        x-=minusVec.x;
        y-=minusVec.y;
    }
    void operator *=(const T multi)
    {
        x*=multi;
        y*=multi;
    }
    void operator /=(const T divi)
    {
        x /= divi;
        y /= divi;
    }

    void operator =(const T target)
    {
        x=target.x;
        y=target.y;
    }

    bool operator >(const Vec2& v)const
    {
        if(x>v.x||y>v.y)
        {return true;}
        return false;
    }
    bool operator <(const Vec2& v)const
    {
        if(x<v.x||y<v.x)
        {return true;}
        return false;
    }
    bool operator ==(const Vec2& v)const
    {
        return(x==v.x&&y==v.y);
    }
    bool operator !=(const Vec2& v)const
    {
        return!(x==v.x&&y==v.y);
    }
	T* ToC()
	{
		return &x;
	}
};
typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec2<short> Vec2s;
#endif