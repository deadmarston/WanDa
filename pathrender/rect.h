#pragma once
#include "hitable.h"
#include "aabb.h"
class xy_rect :public hitable {
public:
	xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material* _mp) :x0(_x0),
		x1(_x1), y0(_y0), y1(_y1), k(_k), mp(_mp) {};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
		return true;
	};

	aabb box;
	material* mp;
	float x0, x1, y0, y1, k;
};

bool xy_rect::hit(const ray& r, float t_min, float t_max, hit_record &rec) const {
	//first,   calculate the z which interacts with r,
	//then,    calculate the x, y according to the z
	//finally, judge whether it's hitted or not by x0, x1, y0, y1
	float t = (k - r.origin().z()) / r.direction().z();

	if (t < t_min || t_max < t) {
		return false;
	}

	float x = r.origin().x() + t*r.direction().x();
	float y = r.origin().y() + t*r.direction().y();

	if (x <= x0 || x1 <= x || y < y0 || y1 <= y) {
		return false;
	}
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;

	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0, 0, 1);

	return true;
}

class yz_rect : public hitable {
public:
	yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material* _mp) :y0(_y0),
		y1(_y1), z0(_z0), z1(_z1), k(_k), mp(_mp) {};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = aabb(vec3(y0, z0, k - 0.0001), vec3(y1, z1, k + 0.0001));
		return true;
	};

	aabb box;
	material* mp;
	float y0, y1, z0, z1, k;
};

bool yz_rect::hit(const ray& r, float t_min, float t_max, hit_record &rec) const {
	//first,   calculate the x which interacts with r,
	//then,    calculate the y, z according to the x
	//finally, judge whether it's hitted or not by y0, y1, z0, z1
	float t = (k - r.origin().x()) / r.direction().x();

	if (t < t_min || t_max < t) {
		return false;
	}

	float y = r.origin().y() + t*r.direction().y();
	float z = r.origin().z() + t*r.direction().z();

	if (y <= y0 || y1 <= y || z < z0 || z1 <= z) {
		return false;
	}
	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;

	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(1, 0, 0);

	return true;
}

class xz_rect : public hitable {
public:
	xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material* _mp) :x0(_x0),
		x1(_x1), z0(_z0), z1(_z1), k(_k), mp(_mp) {};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = aabb(vec3(x0, z0, k - 0.0001), vec3(x1, z1, k + 0.0001));
		return true;
	};

	aabb box;
	material* mp;
	float x0, x1, z0, z1, k;
};

bool xz_rect::hit(const ray& r, float t_min, float t_max, hit_record &rec) const {
	//first,   calculate the x which interacts with r,
	//then,    calculate the y, z according to the x
	//finally, judge whether it's hitted or not by y0, y1, z0, z1
	float t = (k - r.origin().y()) / r.direction().y();

	if (t < t_min || t_max < t) {
		return false;
	}

	float x = r.origin().x() + t*r.direction().x();
	float z = r.origin().z() + t*r.direction().z();

	if (x <= x0 || x1 <= x || z < z0 || z1 <= z) {
		return false;
	}
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;

	rec.mat_ptr = mp;
	rec.p = r.point_at_parameter(t);
	rec.normal = vec3(0, 1, 0);

	return true;
}

class flip_normals : public hitable {
public:
	flip_normals(hitable* _ptr) :ptr(_ptr) {};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record &rec) const{
		if (ptr->hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else {
			return false;
		}
	}
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		return ptr->bounding_box(t0, t1, box);
	}
	hitable* ptr;
};