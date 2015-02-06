// Maths for Game Programming
// Quaternion class - to be completed in lesson
// Jason Colman Qantm London 2009

#include <assert.h>
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "Quaternion.h"

// TODO Default ctor - set to identity
Quaternion::Quaternion()
{
	w_=1.0f;
	x_=0;
	y_=0;
	z_=0;
}
// TODO Ctor - set from axis and angle
Quaternion::Quaternion(Vec3f axis, float angle)
{
	float halfAngle=angle*0.5;
	float sinHalfAngle =sin(halfAngle);
	x_=axis.x*sinHalfAngle;
	y_=axis.y*sinHalfAngle;
	z_=axis.z*sinHalfAngle;
	w_=cos(angle*0.5);
}
// TODO Normalise()
void Quaternion::Normalise()
{
	float sqrLength=x_*x_+y_*y_+z_*z_+w_*w_;
	sqrt(sqrLength);
	x_/=sqrLength;
	y_/=sqrLength;
	z_/=sqrLength;
	w_/=sqrLength;
}

void Quaternion::CreateFromMatrix( float *pTheMatrix, int rowColumnCount)
{
	if(!pTheMatrix || ((rowColumnCount != 3) && (rowColumnCount != 4))) return;

	// This function is used to take in a 3x3 or 4x4 matrix and convert the matrix
	// to a quaternion.  If rowColumnCount is a 3, then we need to convert the 3x3
	// matrix passed in to a 4x4 matrix, otherwise we just leave the matrix how it is.
	// Since we want to apply a matrix to an OpenGL matrix, we need it to be 4x4.

	// Point the matrix pointer to the matrix passed in, assuming it's a 4x4 matrix
	float *pMatrix = pTheMatrix;

	// Create a 4x4 matrix to convert a 3x3 matrix to a 4x4 matrix (If rowColumnCount == 3)
	float m4x4[16] = {0};

	// If the matrix is a 3x3 matrix (which it is for Quake3), then convert it to a 4x4
	if(rowColumnCount == 3)
	{
		// Set the 9 top left indices of the 4x4 matrix to the 9 indices in the 3x3 matrix.
		// It would be a good idea to actually draw this out so you can visualize it.
		m4x4[0]  = pTheMatrix[0];	m4x4[1]  = pTheMatrix[1];	m4x4[2]  = pTheMatrix[2];
		m4x4[4]  = pTheMatrix[3];	m4x4[5]  = pTheMatrix[4];	m4x4[6]  = pTheMatrix[5];
		m4x4[8]  = pTheMatrix[6];	m4x4[9]  = pTheMatrix[7];	m4x4[10] = pTheMatrix[8];

		// Since the bottom and far right indices are zero, set the bottom right corner to 1.
		// This is so that it follows the standard diagonal line of 1's in the identity matrix.
		m4x4[15] = 1;

		// Set the matrix pointer to the first index in the newly converted matrix
		pMatrix = &m4x4[0];
	}

	// The next step, once we made sure we are dealing with a 4x4 matrix, is to check the
	// diagonal of the matrix.  This means that we add up all of the indices that comprise
	// the standard 1's in the identity matrix.  If you draw out the identity matrix of a
	// 4x4 matrix, you will see that they 1's form a diagonal line.  Notice we just assume
	// that the last index (15) is 1 because it is not effected in the 3x3 rotation matrix.

	// Find the diagonal of the matrix by adding up it's diagonal indices.
	// This is also known as the "trace", but I will call the variable diagonal.
	float diagonal = pMatrix[0] + pMatrix[5] + pMatrix[10] + 1;
	float scale = 0.0f;

	// Below we check if the diagonal is greater than zero.  To avoid accidents with
	// floating point numbers, we substitute 0 with 0.00000001.  If the diagonal is
	// great than zero, we can perform an "instant" calculation, otherwise we will need
	// to identify which diagonal element has the greatest value.  Note, that it appears
	// that %99 of the time, the diagonal IS greater than 0 so the rest is rarely used.

	// If the diagonal is greater than zero
	if(diagonal > 0.00000001)
	{
		// Calculate the scale of the diagonal
		scale = float(sqrt(diagonal ) * 2);

		// Calculate the x_, y_, x_ and w_ of the quaternion through the respective equation
		x_ = ( pMatrix[9] - pMatrix[6] ) / scale;
		y_ = ( pMatrix[2] - pMatrix[8] ) / scale;
		z_ = ( pMatrix[4] - pMatrix[1] ) / scale;
		w_ = 0.25f * scale;
	}
	else 
	{
		// If the first element of the diagonal is the greatest value
		if ( pMatrix[0] > pMatrix[5] && pMatrix[0] > pMatrix[10] )  
		{	
			// Find the scale according to the first element, and double that value
			scale  = (float)sqrt( 1.0f + pMatrix[0] - pMatrix[5] - pMatrix[10] ) * 2.0f;

			// Calculate the x_, y_, x_ and w_ of the quaternion through the respective equation
			x_ = 0.25f * scale;
			y_ = (pMatrix[4] + pMatrix[1] ) / scale;
			z_ = (pMatrix[2] + pMatrix[8] ) / scale;
			w_ = (pMatrix[9] - pMatrix[6] ) / scale;	
		} 
		// Else if the second element of the diagonal is the greatest value
		else if ( pMatrix[5] > pMatrix[10] ) 
		{
			// Find the scale according to the second element, and double that value
			scale  = (float)sqrt( 1.0f + pMatrix[5] - pMatrix[0] - pMatrix[10] ) * 2.0f;

			// Calculate the x_, y_, x_ and w_ of the quaternion through the respective equation
			x_ = (pMatrix[4] + pMatrix[1] ) / scale;
			y_ = 0.25f * scale;
			z_ = (pMatrix[9] + pMatrix[6] ) / scale;
			w_ = (pMatrix[2] - pMatrix[8] ) / scale;
		} 
		// Else the third element of the diagonal is the greatest value
		else 
		{	
			// Find the scale according to the third element, and double that value
			scale  = (float)sqrt( 1.0f + pMatrix[10] - pMatrix[0] - pMatrix[5] ) * 2.0f;

			// Calculate the x_, y_, x_ and w_ of the quaternion through the respective equation
			x_ = (pMatrix[2] + pMatrix[8] ) / scale;
			y_ = (pMatrix[9] + pMatrix[6] ) / scale;
			z_ = 0.25f * scale;
			w_ = (pMatrix[4] - pMatrix[1] ) / scale;
		}
	}
}

void Quaternion::ToMatrix(float pMatrix[16]) const
{
	assert(pMatrix);
	pMatrix[ 0] = 1.0f - 2.0f * ( y_ * y_ + z_ * z_ );  
    pMatrix[ 1] = 2.0f * ( x_ * y_ - z_*w_ );  
    pMatrix[ 2] = 2.0f * ( x_ * z_ + y_*w_ );  
    pMatrix[ 3] = 0.0f;

    // Second row
    pMatrix[ 4] = 2.0f * ( x_ * y_ + z_*w_ );  
    pMatrix[ 5] = 1.0f - 2.0f * ( x_ * x_ + z_ * z_ );  
    pMatrix[ 6] = 2.0f * ( y_ * z_ - x_*w_ );  
    pMatrix[ 7] = 0.0f;  

    // Third row
    pMatrix[ 8] = 2.0f * ( x_ * z_ - y_*w_ );  
    pMatrix[ 9] = 2.0f * ( y_ * z_ + x_*w_);  
    pMatrix[10] = 1.0f - 2.0f * ( x_ * x_ + y_ * y_ );  
    pMatrix[11] = 0.0f;  

    // Fourth row
    pMatrix[12] = 0;  
    pMatrix[13] = 0;  
    pMatrix[14] = 0;  
    pMatrix[15] = 1.0f;
}

Quaternion operator*(const Quaternion& p, const Quaternion& q)
{
	Quaternion r;

	r.w_ = p.w_ * q.w_ - p.x_ * q.x_ - p.y_ * q.y_ - p.z_ * q.z_;
	r.x_ = p.w_ * q.x_ + p.x_ * q.w_ + p.y_ * q.z_ - p.z_ * q.y_;
	r.y_ = p.w_ * q.y_ + p.y_ * q.w_ + p.z_ * q.x_ - p.x_ * q.z_;
	r.z_ = p.w_ * q.z_ + p.z_ * q.w_ + p.x_ * q.y_ - p.y_ * q.x_;

	return r;
}

Quaternion Slerp(const Quaternion& from, const Quaternion& to, float t)
{
	float to1[4];
	double omega, cosom, sinom, scale0, scale1;

	// calc cosine
	cosom = from.x_ * to.x_ + from.y_ * to.y_ + from.z_ * to.z_
		+ from.w_ * to.w_;

	// adjust signs (if necessary)
	if (cosom < 0)
	{ 
		cosom = -cosom; 
		to1[0] = - to.x_;
		to1[1] = - to.y_;
		to1[2] = - to.z_;
		to1[3] = - to.w_;
	} 
	else  
	{
		to1[0] = to.x_;
		to1[1] = to.y_;
		to1[2] = to.z_;
		to1[3] = to.w_;
	}

	// calculate coefficients

	static const double DELTA = 0.001;
	if ( (1.0 - cosom) > DELTA ) 
	{
		// standard case (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	} 
	else 
	{        
		// "from" and "to" quaternions are very close 
		//  ... so we can do a linear interpolation
		scale0 = 1.0 - t;
		scale1 = t;
	}

	Quaternion res;

	// calculate final values
	res.x_ = (float)(scale0 * (double)from.x_ + scale1 * (double)to1[0]);
	res.y_ = (float)(scale0 * (double)from.y_ + scale1 * (double)to1[1]);
	res.z_ = (float)(scale0 * (double)from.z_ + scale1 * (double)to1[2]);
	res.w_ = (float)(scale0 * (double)from.w_ + scale1 * (double)to1[3]);

	return res;
}