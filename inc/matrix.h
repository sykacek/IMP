#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include <iomanip>
#include <iterator>

#include "vector.h"

#define X_ROTATION	(1 << 0)
#define Y_ROTATION	(1 << 1)
#define Z_ROTATION	(1 << 2)

#define X_ARRAY(alpha)	{1, 	0,	0,	\
			0, cosf(alpha), -sinf(alpha), \
			0, sinf(alpha), cosf(alpha)}

#define Y_ARRAY(alpha)	{cosf(alpha), 0, -sinf(alpha), \
			0,	1,	0,	\
			sinf(alpha),	0,	cosf(alpha)}

#define Z_ARRAY(alpha)	{cosf(alpha), -sinf(alpha), 0, \
			sinf(alpha), cosf(alpha), 0, \
			0,	0,	1}

extern const float identity_mat[9];

class matrix_t {
public:
	float m[3][3] = {0};
	matrix_t(void);
	matrix_t(float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22);
	matrix_t(const float *src);
	matrix_t(matrix_t& parent);

	/* update methods */
	void update(float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22);

	void update(const float *src);

	/* overload operators */
	friend std::ostream& operator<<(std::ostream& os, matrix_t& mat);

	vect_t operator*(const vect_t& right);
	matrix_t operator*(const float a);

	matrix_t& operator*=(const float a);
	matrix_t& operator*=(const matrix_t& right);

	matrix_t operator/(const float a);
	matrix_t& operator/=(const float a);

	matrix_t operator+(const matrix_t& right);
	matrix_t& operator+=(const matrix_t& right);

	matrix_t operator-(const matrix_t& right);
	matrix_t& operator-=(const matrix_t& right);
};

class rot_t : public matrix_t {
public:
	uint8_t dir;
	rot_t(void);
	rot_t(rot_t& parent);
	rot_t(uint8_t __direction);
	rot_t(float alpha, uint8_t __direction);

	/*methods*/
	void update(float alpha);
	void update(uint8_t __direction, float alpha);
	void update_dir(uint8_t __direction);
};

#endif //__MATRIX_H__
