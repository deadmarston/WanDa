#pragma once
#include "hitable.h"
#include "aabb.h"
#include "texture.h"
#include "material.h"

class isotropic : public material {
public:
	isotropic(texture* a) :albedo(a) {};
	virtual bool scatter(const ray& ir, const hit_record& rec, vec3& attenuation, ray& scattered) const{
		scattered = ray(rec.p, random_in_unit_sphere());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	};
	texture* albedo;
};

class constant_medium : public hitable {
public:
	constant_medium(hitable* h, float d, texture* a) :boundary(h), density(d) {
		phase_function = new isotropic(a);
	};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		return boundary->bounding_box(t0, t1, box);
	}

	hitable* boundary;
	float density;
	material* phase_function;
};

bool constant_medium::hit(const ray& r, float t_min, float t_max, hit_record &rec) const {
	bool db = false;
	hit_record rec1, rec2;
	if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) {
		if (boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2)) {
			if (rec1.t < t_min)
				rec1.t = t_min;
			if (rec2.t > t_max)
				rec2.t = t_max;
			if (rec1.t >= rec2.t)
				return false;
			if (rec1.t < 0)
				rec1.t = 0;
			float distance_inside_boundary = (rec2.t - rec1.t)*r.direction().length();
			float hit_distance = -(1 / density)*log(random_func());
			if (hit_distance < distance_inside_boundary) {
				rec.t = rec1.t + hit_distance / r.direction().length();
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = vec3(1, 0, 0);
				rec.mat_ptr = phase_function;
				return true;
			}
		}
		return false;
	}
}