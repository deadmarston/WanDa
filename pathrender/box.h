#pragma once
#include "hitable.h"
#include"material.h"
#include "aabb.h"
#include "rect.h"
#include "hitable_list.h"

#include <algorithm>

class box : public hitable {
public:
	box() {}
	box(const vec3& p0, const vec3& p1, material *ptr);
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = aabb(pmin, pmax);
		return true;
	}
	vec3 pmin, pmax;
	hitable *list_ptr;
};

box::box(const vec3& p0, const vec3& p1, material *ptr) {
	pmin = p0;
	pmax = p1;
	hitable **list = new hitable*[6];
	list[0] = new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
	list[1] = new flip_normals(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
	list[2] = new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
	list[3] = new flip_normals(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
	list[4] = new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
	list[5] = new flip_normals(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
	list_ptr = new hitable_list(list, 6);
}

bool box::hit(const ray& r, float t0, float t1, hit_record& rec) const {
	return list_ptr->hit(r, t0, t1, rec);
}

class translate : public hitable {
public:
	translate(hitable* _ptr, const vec3& displacement) : ptr(_ptr), offset(displacement){};

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;

	hitable* ptr;
	vec3 offset;
};

bool translate::hit(const ray& r, float t0, float t1, hit_record& rec) const {
	ray nr = ray(r.origin() - offset, r.direction(), r.time());
	if (ptr->hit(nr, t0, t1, rec)) {
		rec.p += offset;
		return true;
	}
	else {
		return false;
	}
}

bool translate::bounding_box(float t0, float t1, aabb& box) const {
	if (ptr->bounding_box(t0, t1, box)) {
		box = aabb(box.min() + offset, box.max() + offset);
		return true;
	}
	else {
		return false;
	}
}

class rotate_y : public hitable {
public:
	rotate_y(hitable* _ptr, float angle): ptr(_ptr){
		//todo: build a new bbox
		float radians = (M_PI / 180.0)*angle;
		sin_theta = sin(radians);
		cos_theta = cos(radians);
		hasbox = ptr->bounding_box(0, 1, bbox);
		vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
		vec3 max(FLT_MIN, FLT_MIN, FLT_MIN);

		//we have two points, bbox.min() and bbox.max()
		float new_x0 = cos_theta*bbox.min().x() + sin_theta*bbox.min().z();
		float new_z0 = -sin_theta*bbox.min().x() + cos_theta*bbox.min().z();

		float new_x1 = cos_theta*bbox.max().x() + sin_theta*bbox.max().z();
		float new_z1 = -sin_theta*bbox.max().x() + cos_theta*bbox.max().z();

		float y0 = bbox.min().y();
		float y1 = bbox.max().y();

		min[0] = std::min(new_x0, new_x1);
		min[1] = std::min(y0, y1);
		min[2] = std::min(new_z0, new_z1);

		max[0] = std::max(new_x0, new_x1);
		max[1] = std::max(y0, y1);
		max[2] = std::max(new_z0, new_z1);

		bbox = aabb(min, max);
	};

	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;

	hitable* ptr;
	aabb bbox;
	bool hasbox;
	float sin_theta;
	float cos_theta;
};

bool rotate_y::hit(const ray& r, float t0, float t1, hit_record& rec) const {
	//todo:
	//first, rotate the ray
	//then check the hit()
	//finally, rotate the rec.p

	vec3 origin = r.origin();
	vec3 direction = r.direction();
	origin[0] = cos_theta*origin[0] - sin_theta*origin[2];
	origin[2] = sin_theta*origin[0] + cos_theta*origin[2];

	direction[0] = cos_theta*direction[0] - sin_theta*direction[2];
	direction[2] = sin_theta*direction[0] + cos_theta*direction[2];

	ray nr = ray(origin, direction, r.time());
	if (ptr->hit(nr, t0, t1, rec)) {
		//first, rotate the rec.p
		rec.p[0] = cos_theta*rec.p[0] + sin_theta*rec.p[2];
		rec.p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];

		//then do it also for rec.normal
		rec.normal[0] = cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
		rec.normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

		return true;
	}
	else {
		return false;
	}
}

bool rotate_y::bounding_box(float t0, float t1, aabb& box) const {
	box = bbox;
	return hasbox;
}