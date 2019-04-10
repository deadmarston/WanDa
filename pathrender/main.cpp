#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"
using namespace std;

//todo: design a test suite for vec3
void test_vec3() {// a tests suite for vec3
	vec3 vec;
}

vec3 color(const ray& r) {
	vec3 unit = unit_vector(r.direction());
	float t = 0.5*(unit.y() + 1.0);
	return t * vec3(0.3, 0.2, 1.0) + (1 - t)*vec3(1.0, 1.0, 1.0);
}

int main()
{
	test_vec3();

	//a hello world demo

	//come on, let's begin with a simple ray tracer demo

	int nx = 200;
	int ny = 100;

	/*
	 #######200######
	 1###############
	 0###############
	 0###############
	 ################
	 */

	//a output file
	ofstream ofs = ofstream("demo.ppm");

	//a header for pmm file
	ofs << "P3\n" << nx << " " << ny << "\n255\n";
		 
	vec3 camera = vec3(0, 0, 0);
	vec3 begin = vec3(-2, -1, -1);
	vec3 horrizontal = vec3(4, 0, 0);
	vec3 vertical = vec3(0, 2, 0);

	for (int j = ny-1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float v = float(j) / ny;
			float u = float(i) / nx;

			vec3 point = begin + u*horrizontal + v*vertical;
			ray r(camera, point);
			vec3 col = color(r);
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