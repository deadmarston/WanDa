#include <iostream>
#include <fstream>
#include <random>
#include <time.h>
#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include "testJson.h"

using namespace std;

//todo: design a test suite for vec3
void test_vec3() {// a tests suite for vec3
	vec3 vec;
}


vec3 color(const ray& r, hitable* world, int depth) {//a kind of shader in our programs
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation*color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit = unit_vector(r.direction());
		float t = 0.5*(unit.y() + 1.0);
		return t * vec3(0.5, 0.7, 1.0) + (1 - t) * vec3(1.0, 1.0, 1.0);
	}
}


hitable_list* random_scene() {
	int n = 500;
	hitable** list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_func();
			vec3 center(a + 0.9 * random_func(), 0.2, b + 0.9*random_func());
			if ((center - vec3(4.0, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {//diffuse
					list[i++] = new moving_sphere(center, center+vec3(0, 0.5*random_func(), 0),0, 1.0, 0.2, new lambertian(vec3(random_func()*random_func(), random_func()*random_func(), random_func()*random_func())));
				}
				else if (choose_mat < 0.95) {//metal
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1 + random_func()), 0.5*(1 + random_func()), 0.5*(1 + random_func())), 0.5*random_func()));
				}
				else {//glass
					list[i++] = new sphere(center, 0.2, new dielectirc(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectirc(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

int main()
{
	//this is for test of json
	//test_main();
	//system("pause");


	//come on, let's begin with a simple ray tracer demo

	//for a random
	srand(unsigned(time(NULL)));

	int nx = 200;
	int ny = 100;
	int ns = 10;
	/*
	 #######800######
	 6###############
	 0###############
	 0###############
	 ################
	 */

	//a output file
	ofstream ofs = ofstream("demo.ppm");

	//a header for pmm file
	ofs << "P3\n" << nx << " " << ny << "\n255\n";
		 
	vec3 lookfrom = vec3(13, 2, 3);
	vec3 lookat = vec3(0, 0, 0);
	vec3 up_vector = vec3(0, 1, 0);
	float fov = 20;	

	float aperture = 0.0;
	float dist_to_focus = 10;

	camera cam = camera(lookfrom, lookat, up_vector, fov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	//hitable* list[5];
	//list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	//list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	//list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	//list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectirc(1.5));
	//list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectirc(1.5));

	hitable_list* world = random_scene();
	//todo: serialization

	for (int j = ny-1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			cout << j << " " << i << endl;

			vec3 col = vec3(0, 0, 0);
			for (int s = 0; s < ns; s++) {

				float v = float(j + random_func()) / ny;
				float u = float(i + random_func()) / nx;

				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}

			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			ofs << ir << " " << ig << " " << ib << " ";
		}
		ofs << endl;
	}

	ofs.close();

	system("pause");
}