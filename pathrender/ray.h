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

float schlick(float cosine, float ref_idx){
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}
vec3 reflect(const vec3& in, const vec3& n) {
	vec3 out = in - 2 * dot(in, n)*n;
	return out;
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt*dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - dt *n) - n*sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
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