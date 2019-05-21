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
#include "bvh.h"
#include "texture.h"
#include "rect.h"
#include "box.h"
#include "volume.h"

#include "testJson.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

/*
				may alumni bless you

		$$$$$$$$$$$$$$$      $$$$$$$$$$$$$$$%.
	&$               &$    =$                $
	$                $$$$$$=                 @&
B=$$                 $$$$$$                   $$&=
$$$$      +1s        $$&-$$       +1s         $$$
$$$$                 $-   $                   $$$
   $                 $    $                  .B
   $                 @    $                 .=
	$                $     $                %
	 $            =$         =$           @&
	  #$$$$$$$$$%              -@$$$$$$$$B

				莫生        莫生
				气         气

				代码辣鸡非我意,
				自己动手分田地;
				你若气死谁如意?
				谈笑风生活长命.

*/

//todo: design a test suite for vec3
void test_vec3() {// a tests suite for vec3
	vec3 vec;
}


vec3 color(const ray& r, hitable* world, int depth) {//a kind of shader in our programs
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return emitted + attenuation*color(scattered, world, depth + 1);
		}
		else {
			return emitted;
		}
	}
	else {
		/*vec3 unit = unit_vector(r.direction());
		float t = 0.5*(unit.y() + 1.0);
		return t * vec3(0.5, 0.7, 1.0) + (1 - t) * vec3(1.0, 1.0, 1.0);*/
		return vec3(0.0, 0.0, 0.0);
	}
}


hitable* random_scene() {
	int n = 500;
	hitable** list = new hitable*[n + 1];
	checker_texture* checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));

	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_func();
			vec3 center(a + 0.9 * random_func(), 0.2, b + 0.9*random_func());
			if ((center - vec3(4.0, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {//diffuse
					list[i++] = new moving_sphere(center, center+vec3(0, 0.5*random_func(), 0),0, 1.0, 0.2, new lambertian(new constant_texture(vec3(random_func()*random_func(), random_func()*random_func(), random_func()*random_func()))));
					//list[i++] = new sphere(center, 0.2, new lambertian(vec3(random_func()*random_func(), random_func()*random_func(), random_func()*random_func())));
				}
				else if (choose_mat < 0.95) {//metal
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1 + random_func()), 0.5*(1 + random_func()), 0.5*(1 + random_func())), 0.5*random_func()));
				}
				else {//glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new bvh_node(list, i, 0.0, 1.0);
}

hitable* two_spheres() {
	//texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	texture* pertext = new noise_texture();
	int n = 50;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(pertext));

	return new hitable_list(list, 2);
}

hitable *two_perlin_spheres() {
	texture *pertext = new noise_texture(4);
	hitable **list = new hitable*[2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	return new hitable_list(list, 2);
}

hitable *earth() {
	int nx, ny, nn;
	//unsigned char *tex_data = stbi_load("tiled.jpg", &nx, &ny, &nn, 0);
	unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	material *mat = new lambertian(new image_texture(tex_data, nx, ny, 0.25));
	return new sphere(vec3(0, 0, 0), 2, mat);
}

hitable* simple_light() {
	texture *pertext = new noise_texture(4);
	hitable **list = new hitable*[4];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	return new hitable_list(list, 4);
}

hitable *cornell_box() {
	hitable **list = new hitable*[8];
	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	return new hitable_list(list, i);
}

hitable *cornell_smoke() {
	hitable **list = new hitable*[8];
	int i = 0;
	material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	hitable *b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	hitable *b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
	list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0.0, 0.0, 0.0)));
	return new hitable_list(list, i);
}

hitable *final() {
	int nb = 20;
	hitable **list = new hitable*[30];
	hitable **boxlist = new hitable*[10000];
	hitable **boxlist2 = new hitable*[10000];
	material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material *ground = new lambertian(new constant_texture(vec3(0.48, 0.83, 0.53)));
	int b = 0;
	for (int i = 0; i < nb; i++) {
		for (int j = 0; j < nb; j++) {
			float w = 100;
			float x0 = -1000 + i*w;
			float z0 = -1000 + j*w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (random_func() + 0.01);
			float z1 = z0 + w;
			boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}
	int l = 0;
	list[l++] = new bvh_node(boxlist, b, 0, 1);
	material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
	list[l++] = new xz_rect(123, 423, 147, 412, 554, light);
	vec3 center(400, 400, 200);
	list[l++] = new moving_sphere(center, center + vec3(30, 0, 0), 0, 1, 50, new lambertian(new constant_texture(vec3(0.7, 0.3, 0.1))));
	list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
	list[l++] = new sphere(vec3(0, 150, 145), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));
	hitable *boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
	list[l++] = boundary;
	list[l++] = new constant_medium(boundary, 0.2, new constant_texture(vec3(0.2, 0.4, 0.9)));
	boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
	list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(vec3(1.0, 1.0, 1.0)));
	int nx, ny, nn;
	unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	material *emat = new lambertian(new image_texture(tex_data, nx, ny));
	list[l++] = new sphere(vec3(400, 200, 400), 100, emat);
	texture *pertext = new noise_texture(0.1);
	list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(pertext));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxlist2[j] = new sphere(vec3(165 * random_func(), 165 * random_func(), 165 * random_func()), 10, white);
	}
	list[l++] = new translate(new rotate_y(new bvh_node(boxlist2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));
	return new hitable_list(list, l);
}



int main()
{
	//this is for test of json
	//test_main();
	//system("pause");

	srand(unsigned(time(NULL)));

	//come on, let's begin with a simple ray tracer demo

	time_t timer1;
	time_t timer2;

	double seconds;
	time(&timer1);

	//for a random
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
		 
	vec3 lookfrom = vec3(278, 278, -800);
	vec3 lookat = vec3(278, 278, 0);
	vec3 up_vector = vec3(0, 1, 0);
	float fov = 40;	

	float aperture = 0.0;
	float dist_to_focus = 10;

	camera cam = camera(lookfrom, lookat, up_vector, fov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	//hitable* list[5];
	//list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	//list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	//list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	//list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectirc(1.5));
	//list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectirc(1.5));
	//hitable* world = two_spheres();
	hitable* world = final();
	//hitable* world = random_scene();
	//todo: serialization

	for (int j = ny-1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			//cout << j << " " << i << endl;

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

	time(&timer2);
	seconds = difftime(timer2, timer1);
	cout << "elcapsed: " << seconds << endl;

	system("pause");
}