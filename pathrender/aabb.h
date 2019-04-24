#pragma once
#include "vec3.h"
#include "ray.h"

#include <math.h>

inline float ffmin(float a, float b) { return a < b? a : b; }
inline float ffmax(float a, float b) { return a < b? b : a; }

class aabb {
public:
	aabb() {}
	aabb(const vec3& mi, const vec3& ma) { _min = mi; _max = ma; };
	vec3 min() const { return _min; };
	vec3 max() const { return _max; };

	bool hit(const ray& r, float tmin, float tmax) const {
		//for (int a = 0; a < 3; a++) {
		//	float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);
		//	float t1 = ffmax((_max[a] - r.origin()[a]) / r.direction()[a], (_min[a] - r.origin()[a]) / r.direction()[a]);
		//	tmin = ffmax(tmin, t0);
		//	tmax = ffmin(tmax, t1);
		//	if (tmax <= tmin)
		//		return false;
		//}
		//return true;
		
		//a andrew kensier's method
		for (int a = 0; a < 3; a++) {
			float division = 1.0 / r.direction()[a];
			float t0 = (_min[a] - r.origin()[a]) * division;
			float t1 = (_max[a] - r.origin()[a]) * division;
			if (division < 0.0f) {
				swap(t0, t1);
			}
			tmin = tmin > t0 ? tmin : t0;
			tmax = tmax < t1 ? tmax : t1;
			if (tmax <= tmin)
				return false;
		}
		return true;
	}

	vec3 _min;
	vec3 _max;
};

aabb surrounding_box(aabb box0, aabb box1) {
	return aabb(vec3(fmin(box0._min.x(), box1._min.x()),
					fmin(box0._min.y(), box1._min.y()),
					fmin(box0._min.z(), box1._min.z())),
				vec3(fmax(box0._max.x(), box1._max.x()),
					fmax(box0._max.y(), box1._max.y()),
					fmax(box0._max.z(), box1._max.z())));
}