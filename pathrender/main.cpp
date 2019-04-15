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


int main()
{
	test_vec3();

	//a hello world demo

	//come on, let's begin with a simple ray tracer demo

	//for a random
	srand(unsigned(time(NULL)));

	int nx = 200;
	int ny = 100;
	int ns = 100;
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
		 
	vec3 origin = vec3(0, 0, 0);
	vec3 begin = vec3(-2, -1, -1);
	vec3 horrizontal = vec3(4, 0, 0);
	vec3 vertical = vec3(0, 2, 0);
	camera cam = camera(origin, begin, horrizontal, vertical);

	hitable* list[4];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectirc(1.5));
	//list[4] = new sphere(vec3(-1, 0, -1), 0.45, new dielectirc(1.5));

	hitable_list* world = new hitable_list(list, 4);

	for (int j = ny-1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {

			vec3 col = vec3(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				//random engine?
				//default_random_engine e;
				//uniform_real_distribution<float> uni(0, 0.999999999999999);

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