#include "matrix.h"

const float identity_mat[9] = {
	1.0f, 0.0f, 0.0f,
	0.0f,1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

matrix_t::matrix_t(void){
	return;
}

matrix_t::matrix_t(const float *src){
	memcpy(m, src, 9 * sizeof(float));
}

matrix_t::matrix_t(matrix_t &parent){
	memcpy(m, parent.m, 9 * sizeof(float));
}

void matrix_t::update(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22){
	m[0][0] = a00;
	m[0][1] = a01;
	m[0][2] = a02;
	
	m[1][0] = a01;
	m[1][1] = a11;
	m[1][2] = a12;
	
	m[2][0] = a20;
	m[2][1] = a21;
	m[2][2] = a22;
}

void matrix_t::update(const float *src){
	memcpy(m, src, 9 * sizeof(float));
}

vect_t matrix_t::operator*(const vect_t &right){
	vect_t ret(x_axis);

	for(int i = 0; i < 3; ++i)
		ret.m[i] = m[i][0] * right.m[0] + m[i][1] * right.m[1] + right.m[2] * m[i][2];

	return ret;
}

matrix_t matrix_t::operator*(const float a){
	matrix_t ret(identity_mat);
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			ret.m[i][j] = a * m[i][j];
	return ret;
}

matrix_t &matrix_t::operator*=(const float a){
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			m[i][j] *= a;

	return *this;
}

matrix_t &matrix_t::operator*=(const matrix_t &right){
	matrix_t c(*this);

	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			m[i][j] = 0;
			for(int l = 0; l < 3; ++l)
				m[i][j] += c.m[i][l] * right.m[l][j];
		}
	}		

	return *this;
}

matrix_t matrix_t::operator/(const float a){
	matrix_t ret(*this);
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			ret.m[i][j] /= a;

	return ret;
}

matrix_t &matrix_t::operator/=(const float a){
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			m[i][j] /= a;

	return *this;
}

matrix_t matrix_t::operator+(const matrix_t &right){
	matrix_t ret(identity_mat);
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			ret.m[i][j] = m[i][j] + right.m[i][j];

	return ret;
}

matrix_t& matrix_t::operator+=(const matrix_t &right){
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			m[i][j] += right.m[i][j];
	
	return *this;

}

matrix_t matrix_t::operator-(const matrix_t &right){
	matrix_t ret(identity_mat);
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			ret.m[i][j] = m[i][j] - right.m[i][j];

	return ret;
}

matrix_t &matrix_t::operator-=(const matrix_t &right){
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			m[i][j] -= right.m[i][j];

	return *this;
}

matrix_t::matrix_t(float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22)
{
	m[0][0] = a00;
	m[0][1] = a01;
	m[0][2] = a02;
	
	m[1][0] = a01;
	m[1][1] = a11;
	m[1][2] = a12;
	
	m[2][0] = a20;
	m[2][1] = a21;
	m[2][2] = a22;
}

std::ostream &operator<<(std::ostream &os, matrix_t &mat){
	return os << std::fixed << std::setprecision(FSTREAM_PRECISION) << 
		mat.m[0][0] << " " << mat.m[0][1] << " " << mat.m[0][2] << "\n" << 
		mat.m[1][0] << " " << mat.m[1][1] << " " << mat.m[1][2] << "\n" << 
		mat.m[2][0] << " " << mat.m[2][1] << " " << mat.m[2][2] << "\n"; 

	}

rot_t::rot_t(void){
	;	
}

rot_t::rot_t(rot_t &parent){
	memcpy(m, parent.m, 9 * sizeof(float));
}

rot_t::rot_t(uint8_t __direction){
	dir = __direction;
}

rot_t::rot_t(float alpha, uint8_t __direction){
	dir = __direction;
/*
	switch(dir){
		case(X_ROTATION):{
			float src[9] = X_ARRAY(alpha);
			memcpy(m, src , sizeof(float) * 9);

			break;
		}
		case(Y_ROTATION):{
			float src[9] = Y_ARRAY(alpha);
			memcpy(m, src , sizeof(float) * 9);

			break;
		}
		case(Z_ROTATION):{
			float src[9] = Z_ARRAY(alpha);
			memcpy(m, src , sizeof(float) * 9);

			break;
		}
		default:{
			puts("Invalid flag");
			memcpy(m, identity_mat , sizeof(float) * 9);

		}
	}
*/
	update(alpha);
}


void rot_t::update(float alpha){
	switch(dir){
		case(X_ROTATION):{
			float src[9] = X_ARRAY(alpha);
			memcpy(m, src , sizeof(float) * 9);

			break;
		}
		case(Y_ROTATION):{
			float src[9] = Y_ARRAY(alpha);
			memcpy(m, src , sizeof(float) * 9);

			break;
		}
		case(Z_ROTATION):{
			float src[9] = Z_ARRAY(alpha);
			memcpy(m, src , sizeof(float) * 9);

			break;
		}
		default:{
			puts("Invalid flag");
			memcpy(m, identity_mat , sizeof(float) * 9);

		}
	}
}

void rot_t::update(uint8_t __direction, float alpha){
	dir = __direction;
	update(alpha);
}

void rot_t::update_dir(uint8_t __direction){
	dir = __direction;
}
