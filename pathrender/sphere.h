#pragma once
#include "hitable.h"
#include "texture.h"

class sphere : public hitable {
public:
	sphere() {};
	sphere(vec3 c, float rad, material* mm) : center(c), radius(rad), mat(mm) {};
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	vec3 center;
	float radius;
	material* mat;
};

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
	box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}

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
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = vec3(rec.p - center)/radius;
			return true;
		}
		temp = (-coeffB + sqrt_discriminant) / coeffA;
		if (temp < t_max && t_min < temp) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = vec3(rec.p - center) / radius;
			return true;
		}
	}
	return false;
}

class moving_sphere : public hitable {
public:
	moving_sphere();
	moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material* m) : 
		center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat(m){}
	
	virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	vec3 center(float time) const;
	vec3 center0;
	vec3 center1;
	float time0;
	float time1;
	float radius;
	material* mat;
};

vec3 moving_sphere::center(float time) const{
	return center0 + (time - time0) / (time1 - time0) * (center1 - center0);
}

bool moving_sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const{
	vec3 dir = r.direction();
	vec3 oc = r.origin() - center(r.time());
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
			get_sphere_uv((rec.p - center(r.time()))/ radius, rec.u, rec.v);
			rec.normal = vec3(rec.p - center(r.time())) / radius;
			return true;
		}
		temp = (-coeffB + sqrt_discriminant) / coeffA;
		if (temp < t_max && t_min < temp) {
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			get_sphere_uv((rec.p - center(r.time()))/ radius, rec.u, rec.v);
			rec.normal = vec3(rec.p - center(r.time())) / radius;
			return true;
		}
	}
	return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb& box) const {
	aabb box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
	aabb box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
	box = surrounding_box(box0, box1);
	return true;
}