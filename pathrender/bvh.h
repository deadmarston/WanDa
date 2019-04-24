#pragma once
#include "ray.h"
#include "vec3.h"
#include "aabb.h"
#include "hitable.h"

#include <stdlib.h>

//for the qsort of bvh
int compare_by_x(const void* a, const void* b) {
	//todo:
	aabb a_box, b_box;
	hitable* ah = *(hitable**)a;
	hitable* bh = *(hitable**)b;

	if (!ah->bounding_box(0, 0, a_box) || !bh->bounding_box(0, 0, b_box)) {
		//oop, error
	}
	if (a_box.min().x() - b_box.min().x() < 0.0)
		return -1;
	else
		return 1;
}

int compare_by_y(const void* a, const void* b) {
	//todo:
	aabb a_box, b_box;
	hitable* ah = *(hitable**)a;
	hitable* bh = *(hitable**)b;

	if (!ah->bounding_box(0, 0, a_box) || !bh->bounding_box(0, 0, b_box)) {
		//oop, error
	}
	if (a_box.min().y() - b_box.min().y() < 0.0)
		return -1;
	else
		return 1;

}

int compare_by_z(const void* a, const void* b) {
	//todo:
	aabb a_box, b_box;
	hitable* ah = *(hitable**)a;
	hitable* bh = *(hitable**)b;

	if (!ah->bounding_box(0, 0, a_box) || !bh->bounding_box(0, 0, b_box)) {
		//oop, error
	}
	if (a_box.min().z() - b_box.min().z() < 0.0)
		return -1;
	else
		return 1;

}

class bvh_node : public hitable {
public:
	bvh_node();
	bvh_node(hitable** l, int n, float time0, float time1);

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;

	hitable* left;
	hitable* right;

	aabb box;
};

bvh_node::bvh_node(hitable** l, int n, float time0, float time1) {
	//todo: build the bvh tree with the hitable**

	//first, decide the axis to divide the hitable objects
	int tmp = random_func() * 3;
	if (tmp == 0){//by x
		qsort(l, n, sizeof(hitable*), compare_by_x);
	}
	else if (tmp == 1){//by y
		qsort(l, n, sizeof(hitable*), compare_by_y);
	}
	else if (tmp == 2){//by z
		qsort(l, n, sizeof(hitable*), compare_by_z);
	}
	else {
		//error
	}

	//then we try to divide it into left and right
	if (n == 0) {
		//oop, error
	}
	else if (n == 1) {
		left = l[0];
		right = l[0];
	}
	else if (n == 2) {
		left = l[0];
		right = l[1];
	}
	else {//build the bvh_node recursively
		left = new bvh_node(l, n / 2, time0, time1);
		right = new bvh_node(l + n / 2, (n+1)/2, time0, time1);
	}
	aabb left_box, right_box;
	if (!left->bounding_box(time0, time1, left_box) || !right->bounding_box(time0, time1, right_box)) {
		//oop, error
		//todo:
	}
	box = surrounding_box(left_box, right_box);
}

bool bvh_node::hit(const ray& r, float t_min, float t_max, hit_record &rec) const {
	//todo: hit the bvh recursively
	//first check with box
	if (!box.hit(r, t_min, t_max)) {
		return false;
	}
	hit_record tmp1, tmp2;
	bool leftHit = left->hit(r, t_min, t_max, tmp1);
	bool rightHit = right->hit(r, t_min, t_max, tmp2);

	if (leftHit && rightHit) {
		if (tmp1.t < tmp2.t) {
			rec = tmp1;
		}
		else {
			rec = tmp2;
		}
	}
	else if (leftHit) {
		rec = tmp1;
	}
	else if (rightHit){
		rec = tmp2;
	}
	else {
		return false;
	}
	return true;
}

bool bvh_node::bounding_box(float t0, float t1, aabb& b) const {
	//return the bounding_box
	b = box;
	return true;
}