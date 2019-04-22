#pragma once
#include "ray.h"
#include "hitable.h"
#include "vec3.h"

class material {
public:
	virtual bool scatter(const ray& ir, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	lambertian(const vec3& a) : albedo(a) {};
	virtual bool scatter(const ray& ir, const hit_record& rec, vec3& attenutaion, ray& scattered) const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p, ir.time());
		attenutaion = albedo;
		return true;
	}

	vec3 albedo;
};

class metal :public material {
public:
	metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; };
	virtual bool scatter(const ray& ir, const hit_record& rec, vec3& attenuation, ray& scattered) const{
		vec3 reflected = reflect(unit_vector(ir.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(), ir.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	vec3 albedo;
	float fuzz;
};

class dielectirc : public material {
public:
	dielectirc(float ri) :ref_idx(ri) {};
	virtual bool scatter(const ray& r, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 outward_normal;
		vec3 reflected = reflect(r.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		float reflect_prob;
		float cosine;

		if (dot(r.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r.direction(), rec.normal) / r.direction().length();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r.direction(), rec.normal) / r.direction().length();
		}

		if (refract(r.direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			reflect_prob = 1.0;
		}
		if (random_func() < reflect_prob) {
			scattered = ray(rec.p, reflected, r.time());
		}
		else {
			scattered = ray(rec.p, refracted, r.time());
		}
		return true;
	}
	float ref_idx;
};