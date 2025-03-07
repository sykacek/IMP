#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cstdint>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <math.h>

#define FSTREAM_PRECISION		(3)

extern const float x_axis[3];
extern const float y_axis[3];
extern const float y_axis[3];

class vect_t {
public:
	float m[3] = {0};
	
	/* constructors */
	vect_t(void);
	vect_t(const float *src);
	vect_t(float a1, float a2, float a3);
	vect_t(vect_t &parent);


	/* set and get methods */
	void update(float *src);
	void update(float a1, float a2, float a3);
	float *get(void);
	void round(const uint8_t prec);

	/* operator overloads */
	friend std::ostream& operator<<(std::ostream& os, const vect_t& vect);

	vect_t operator+(const vect_t& right);
	vect_t& operator+=(const vect_t& right);

	vect_t operator-(const vect_t& right);
	vect_t& operator-=(const vect_t& right);

	vect_t operator*(const float a);
	vect_t& operator*=(const float a);

	vect_t operator/(const float a);
	vect_t& operator/=(const float a);
};


#endif