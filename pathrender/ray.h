#pragma once
#include "vec3.h"

class ray {
public:
	ray() {};
	ray(vec3& A, vec3& B) { a = A; b = B; };

	vec3 origin() const { return a; };
	vec3 direction() const { return b; }
	vec3 point_at_parameter(float t) const { return a + t*b; };

	vec3 a;
	vec3 b;
};