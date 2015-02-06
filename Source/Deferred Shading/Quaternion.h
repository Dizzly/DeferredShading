// Maths for Game Programming
// Quaternion class - to be completed in lesson
// Jason Colman Qantm London 2009

#ifndef QUATERNION_H_INCLUDED
#define QUATERNION_H_INCLUDED

#include "Vec3.h"

class Quaternion  
{
public:
    // TODO Default ctor - set to identity
	Quaternion();
    // TODO Ctor - set from axis and angle
	Quaternion(Vec3f axis, float angle);
    // TODO Normalise()
	void Normalise();

    // Convert to column-major Matrix
    void ToMatrix(float matrix[16]) const;

	void CreateFromMatrix( float* pMatrix, int rowColounCount);

    // Interpolate between two Quaternions
    // 0 <= t <= 1
    friend Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);

    friend Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

private:
    float w_;
    float x_;
    float y_;
    float z_;
};

#endif 