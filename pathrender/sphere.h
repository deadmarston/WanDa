#pragma once
#include "hitable.h"

class sphere : public hitable {
public:
	sphere() {};
	sphere(vec3 c, float rad, material* mm) : center(c), radius(rad), mat(mm) {};
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	vec3 center;
	float radius;
	material* mat;
};

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const{
	vec3 dir = r.direction();
	vec3 oc = r.origin() - center;
	float coeffA = dot(dir, dir);
	float coeffB = dot(dir, oc);
	float coeffC = dot(oc, oc) - radius * radius;

	float discriminant = coeffB*coeffB - coeffA*coeffC;

	if (discriminant > 0) {
		float sqrt_discriminant = sqrt(discriminant);
		float temp = (-coeffB - sqrt_discriminant) / coeffA;

		rec.mat_ptr = mat;
		if (temp < t_max && t_min < temp) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = vec3(rec.p - center)/radius;
			return true;
		}
		temp = (-coeffB + sqrt_discriminant) / coeffA;
		if (temp < t_max && t_min < temp) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = vec3(rec.p - center) / radius;
			return true;
		}
	}
	return false;
}