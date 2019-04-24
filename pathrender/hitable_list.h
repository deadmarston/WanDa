#pragma once
#include "hitable.h"


class hitable_list : public hitable {
public:
	hitable_list() {};
	hitable_list(hitable** li, int size) : list(li), sizeOfList(size){};

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	hitable** list;
	int sizeOfList;

	//todo:
	//serialize the data of objects, try to reuse it in next time
	//void serialization(ofstream& ofs) const;
	//void deserialization(ifstream& ifs);
};



bool hitable_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const{
	bool hitAnything = false;
	float closest_so_far = t_max;
	hit_record temp_hit;
	for (int i = 0; i < sizeOfList; i++) {
		if (list[i]->hit(r, t_min, t_max, temp_hit)) {
			hitAnything = true;
			if (temp_hit.t < closest_so_far) {
				closest_so_far = temp_hit.t;
				rec = temp_hit;
			}
		}
	}
	return hitAnything;
}

bool hitable_list::bounding_box(float t0, float t1, aabb& box) const {
	//todo:
	return true;
}