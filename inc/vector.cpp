#include "vector.h"

const float x_axis[3] = {1, 0, 0};
const float y_axis[3] = {0, 1, 0};
const float z_axis[3] = {0, 0, 1};

vect_t::vect_t(void){
	;
}

vect_t::vect_t(const float *src){
	memcpy(m, src, 3 * sizeof(float));
}

vect_t::vect_t(float a1, float a2, float a3){
	m[0] = a1;
	m[1] = a2;
	m[2] = a3;
}

vect_t::vect_t(vect_t &parent){
	memcpy(m, parent.m, 3 * sizeof(float));
}

void vect_t::update(float *src){
	memcpy(m, src, 3 * sizeof(float));
}

void vect_t::update(float a1, float a2, float a3){
	m[0] = a1;
	m[1] = a2;
	m[2] = a3;
}

float *vect_t::get(void){
	return m;
}

void vect_t::round(const uint8_t prec){
	const float factor = powf(10.0f, (float)prec);
	for(int i = 0; i < 3; ++i)
		m[i] = roundf(m[i] * factor) / factor; 
}

vect_t vect_t::operator+(const vect_t &right){
	vect_t ret(*this);
	for(int i = 0; i < 3; ++i)
		ret.m[i] += right.m[i];

	return ret;
}

vect_t &vect_t::operator+=(const vect_t &right){
	for(int i = 0; i < 3; ++i)
		m[i] += right.m[i];

	return *this;
}

vect_t vect_t::operator-(const vect_t &right){
	vect_t ret(*this);
	for(int i = 0; i < 3; ++i)
		ret.m[i] -= right.m[i];

	return ret;
}

vect_t &vect_t::operator-=(const vect_t &right){
	for(int i = 0; i < 3; ++i)
		m[i] -= right.m[i];

	return *this;

}

vect_t vect_t::operator*(const float a){
	vect_t ret(*this);
	for(int i = 0; i < 3; ++i)
		ret.m[i] *= a;

	return ret;

}

vect_t &vect_t::operator*=(const float a){
	for(int i = 0; i < 3; ++i)
		m[i] *= a;

	return *this;
}
vect_t vect_t::operator/(const float a)
{
	vect_t ret(*this);
	for(int i = 0; i < 3; ++i)
		ret.m[i] /= a;

	return ret;
}

vect_t &vect_t::operator/=(const float a){
	for(int i = 0; i < 3; ++i)
		m[i] /= a;

	return *this;
}

std::ostream &operator<<(std::ostream &os, const vect_t &vect){
	return os << std::fixed << std::setprecision(FSTREAM_PRECISION) << vect.m[0] << " " << vect.m[1] << " " << vect.m[2] << "\n";
}