#pragma once
#include <iostream>
using namespace std;

class vec3 {
public:
	float v[3];
	vec3() { v[0] = 0; v[1] = 0; v[2] = 0; };
	vec3(float xx, float yy, float zz) { v[0] = xx; v[1] = yy; v[2] = zz; };
	inline float x() const { return v[0]; };
	inline float y() const { return v[1]; };
	inline float z() const { return v[2]; };

	inline float r() const { return v[0]; };
	inline float g() const { return v[1]; };
	inline float b() const { return v[2]; };

	inline const vec3& operator+() const { return *this; };
	inline vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); };
	inline float operator[](int i) const { return v[i]; };
	inline float& operator[](int i) { return v[i]; };

	inline vec3& operator+=(const vec3 &v2);
	inline vec3& operator-=(const vec3 &v2);
	inline vec3& operator*=(const vec3 &v2);
	inline vec3& operator/=(const vec3 &v2);
	inline vec3& operator*=(const float t);
	inline vec3& operator/=(const float t);

	inline float length() const 
		{ return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); };
	inline float square_length() const
		{ return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];};

	inline void make_unit_vector();
		
};

inline std::istream& operator>> (std::istream &is, vec3 &t) {
	is >> t.v[0] >> t.v[1] >> t.v[2];
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
	os << t.v[0] << t.v[1] << t.v[2];
}

inline void vec3::make_unit_vector() {
	float k = 1 / this->length();
	v[0] *= k;
	v[1] *= k;
	v[2] *= k;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.v[0] * v2.v[0], v1.v[1] * v2.v[1], v2.v[2] * v2.v[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.v[0] / v2.v[0], v1.v[1] / v2.v[1], v1.v[2] / v2.v[2]);
}

inline vec3 operator*(const vec3 &v1, float t) {
	return vec3(v1.v[0] * t, v1.v[1] * t, v1.v[2] * t);
}

inline vec3 operator/(vec3 v1, float t) {
	return vec3(v1.v[0] / t, v1.v[1] / t, v1.v[2] / t);
}

inline vec3 operator*(float t, const vec3 &v1) {
	return vec3(v1.v[0] * t, v1.v[1] * t, v1.v[2] * t);
}

inline float dot(const vec3 &v1, const vec3 &v2) {
	return v1.v[0]*v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.v[1]*v2.v[2]-v1.v[2]*v2.v[1], -(v1.v[0]*v2.v[2]-v1.v[2]*v1.v[0]), v1.v[0]*v2.v[1]-v1.v[1]*v2.v[0]);
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

inline vec3& vec3::operator+=(const vec3 &v2) {
	v[0] += v2.v[0];
	v[1] += v2.v[1];
	v[2] += v2.v[2];
}

inline vec3& vec3::operator-=(const vec3 &v2) {
	v[0] -= v2.v[0];
	v[1] -= v2.v[1];
	v[2] -= v2.v[2];
}

inline vec3& vec3::operator*=(const vec3 &v2) {
	v[0] *= v2.v[0];
	v[1] *= v2.v[1];
	v[2] *= v2.v[2];
}

inline vec3& vec3::operator/=(const vec3 &v2) {
	v[0] /= v2.v[0];
	v[1] /= v2.v[1];
	v[2] /= v2.v[2];
}

inline vec3& vec3::operator*=(const float f) {
	v[0] *= f;
	v[1] *= f;
	v[2] *= f;
}

inline vec3& vec3::operator/=(const float f) {
	v[0] /= f;
	v[1] /= f;
	v[2] /= f;
}
