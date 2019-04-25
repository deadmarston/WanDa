#pragma once
#include "vec3.h"
#include "perlin.h"

#define M_PI 3.14159265358979323846

void get_sphere_uv(const vec3& p, float &u, float &v) {
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	//my version
	u = 1-(phi + M_PI) / (2 *  M_PI);
	v = (M_PI / 2 - theta) / M_PI;

	//p.s 's version
	//u = 1 - (phi + M_PI) / (2 * M_PI);
	//v = (theta + M_PI / 2) / M_PI;
}

class texture {
public:
	virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture{
public:
	constant_texture() {};
	constant_texture(vec3 c) : color(c) {};

	virtual vec3 value(float u, float v, const vec3& p) const {
		return color;
	}

	vec3 color;
};

class checker_texture : public texture {
public:
	checker_texture() {};
	checker_texture(texture* t0, texture* t1) : even(t0), odd(t1) {};

	virtual vec3 value(float u, float v, const vec3& p) const {
		float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0) {
			return odd->value(u, v, p);
		}
		else {
			return even->value(u, v, p);
		}
	}

	texture *odd;
	texture *even;
};

class noise_texture :public texture {
public:
	noise_texture() {};
	noise_texture(float sc) : scale(sc) {}
	virtual vec3 value(float u, float v, const vec3& p) const{
		return vec3(1.0, 1.0, 1.0) * 0.5 * (1 + sin(scale*p.z() + 10* noise.turb(p)));
	}
	perlin noise;
	float scale;
};

class image_texture : public texture {
public:
	image_texture() {}
	image_texture(unsigned char* pixels, int A, int B, float off_x=0.0, float off_y=0.0) : data(pixels), nx(A), ny(B), offset_x(off_x), offset_y(off_y) {};

	virtual vec3 value(float u, float v, const vec3& p) const;

	unsigned char* data;
	int nx, ny;
	float offset_x, offset_y;
};

vec3 image_texture::value(float u, float v, const vec3& p) const {
	int i = (u+offset_x)*nx;
	int j = (v+offset_y)*ny;
	if (i < 0) i += nx;//work in a repeat mode
	if (j < 0) j += ny;
	if (i > nx - 1) i -= nx;
	if (j > ny - 1) j -= ny ;
	float r = int(data[3 * i + 3 * j*nx    ]) / 255.0;
	float g = int(data[3 * i + 3 * j*nx + 1]) / 255.0;
	float b = int(data[3 * i + 3 * j*nx + 2]) / 255.0;
	return vec3(r, g, b);
}