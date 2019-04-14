#pragma once
#include "ray.h"

class camera {
public:
	vec3 origin;
	vec3 left_down_point;
	vec3 horizontal;
	vec3 vertical;

	camera() {};
	camera(vec3 ori, vec3 ld, vec3 hor, vec3 vert) : origin(ori), left_down_point(ld), horizontal(hor), vertical(vert) {};

	ray get_ray(float u, float v);
};

ray camera::get_ray(float u, float v) {
	vec3 direction = left_down_point + u * horizontal + v * vertical - origin;
	return ray(origin, direction);
}