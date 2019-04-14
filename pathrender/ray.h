#pragma once
#include "vec3.h"

class ray {
public:
	ray() {};
	ray(const vec3& A, const vec3& B) { a = A; b = B; };

	vec3 origin() const { return a; };
	vec3 direction() const { return b; }
	vec3 point_at_parameter(float t) const { return a + t*b; };

	vec3 a;
	vec3 b;
};

vec3 reflect(const vec3& in, const vec3& n) {
	vec3 out = in - 2 * dot(in, n)*n;
	return out;
}

float random_func(int precision = 10000, bool leftIncluded = false) {
	return (rand() % (precision + (leftIncluded ? 1 : 0))) / float(precision);
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2 * vec3(random_func(), random_func(), random_func()) - vec3(1.0, 1.0, 1.0);
	} while (p.square_length() >= 1.0);
	return p;
}