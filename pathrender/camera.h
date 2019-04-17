#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "ray.h"

class camera {
public:
	vec3 origin;
	vec3 left_down_point;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;


	float lens_radius;

	camera() {};
	/*camera(vec3 ori, vec3 ld, vec3 hor, vec3 vert) : origin(ori), left_down_point(ld), horizontal(hor), vertical(vert) {};
	camera(float vfov, float aspect) {
		float theta = vfov * M_PI / 180.0;
		float height = tan(theta / 2.0);
		float width = aspect * height;

		origin = vec3(0, 0, 0);
		left_down_point = vec3(-width, -height, -1);
		horizontal = vec3(width * 2, 0, 0);
		vertical = vec3(0, height * 2, 0);
	};*/
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) {
		lens_radius = aperture / 2;

		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

		origin = lookfrom;
		
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		left_down_point = origin - w*focus_dist - half_width*focus_dist*u - half_height*focus_dist*v;
		horizontal = half_width * 2 * u *focus_dist;
		vertical = half_height * 2 * v * focus_dist;
	}

	ray get_ray(float s, float t);
};

ray camera::get_ray(float s, float t) {
	vec3 rd = lens_radius * random_in_unit_disk();
	vec3 offset = u*rd.x() + v*rd.y();
	return ray(origin + offset, left_down_point + s * horizontal + t*vertical -origin - offset);
}