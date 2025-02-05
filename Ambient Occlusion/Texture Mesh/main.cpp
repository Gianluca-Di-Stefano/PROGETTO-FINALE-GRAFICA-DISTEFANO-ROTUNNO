#include <iostream>
#include <time.h>
#include <stdio.h>
#include "SDL.h" 
#include "SDL_image.h"
#include "float.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "geometry.h"
#include "cylinder.h"
#include "sphere.h"
#include "hittable.h"
#include "hittable_list.h"
#include "instance.h"
#include "mesh.h"
#include "raster.h"
#include "object.h"
#include "camera.h"
#include "color.h"
#include "texture.h"
#include "sampler.h"
#include "regular.h"
#include "multiJittered.h"
#include "pure_random.h"
#include "ambient_occluder.h"
#include "light.h"

using namespace std;

int init(int width, int height)  {
	/* // Initialize SDL2. */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* Create the window where we will draw. */
	window = SDL_CreateWindow("Texture Mesh", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	/* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	return 0;
}

void close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

color getColor(string colore) {

	if (colore == "red") {
		return color(1.00f, 0.00f, 0.00f);
	}
	if (colore == "green") {
		return color(0.00f, 1.00f, 0.00f);
	}
	if (colore == "blue") {
		return color(0.00f, 0.00f, 1.00f);
	}
	if (colore == "gray") {
		return color(0.50f, 0.50f, 0.50f);
	}
	if (colore == "white") {
		return color(1.00f, 1.00f, 1.00f);
	}
	if (colore == "black") {
		return color(0.00f, 0.00f, 0.00f);
	}
	if (colore == "darkgray") {
		return color(0.25f, 0.25f, 0.25f);
	}
	if (colore == "lightgray") {
		return color(0.75f, 0.75f, 0.75f);
	}
	if (colore == "slategray") {
		return color(0.40f, 0.50f, 0.56f);
	}
	if (colore == "magenta") {
		return color(1.00f, 0.00f, 1.00f);
	}
	if (colore == "cyan") {
		return color(0.00f, 1.00f, 1.00f);
	}
	if (colore == "brown") {
		return color(0.60f, 0.40f, 0.12f);
	}
	if (colore == "yellow") {
		return color(0.65f, 0.65f, 0.00f);
	}

}

void sphereMultiJittered(camera cam, hittable_list world) {
	
	//Luce ambient occluder
	ambient_occluder* occluder_ptr = new ambient_occluder(getColor("white"),getColor("white"),getColor("white"),1.0);
	occluder_ptr->set_min_amount(0.0);

	//Cube base
	mesh* cube = new mesh("../models/cube.obj", "../models/");
	texture* colore_cubo = new image_texture("../models/grigio.jpg");
	material* m_cube = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_cubo = make_shared<instance>(cube, m_cube);
	instance_ptr_cubo->scale(40.0, 1.0, 40.0);
	instance_ptr_cubo->translate(0.0f, -0.5f, 0.0f);
	m_cube->texture = colore_cubo;
	world.add(instance_ptr_cubo);

	//Sfera principale 
	mesh* sfera_obj = new mesh("../models/sfera.obj", "../models/");
	texture* colore_sfera_obj = new image_texture("../models/rosso.jpg");
	material* m_sfera_obj = new material(getColor("red"), getColor("red"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_sfera_obj = make_shared<instance>(sfera_obj, m_sfera_obj);
	instance_ptr_sfera_obj->scale(0.3, 0.3, 0.3);
	m_sfera_obj->texture = colore_sfera_obj;
	world.add(instance_ptr_sfera_obj);

	//Camera
	cam.lookfrom = point3(0, 5, 10);
	cam.lookat = point3(0, 0.5, 0);

	int num_samples = 0;

	std::vector<int> vec_samples = { 1, 16, 64, 128, 256 };

	for (int i = 0; i < vec_samples.size(); i++) {
		//Sampler setting
		num_samples = vec_samples[i];
		cam.samples_per_pixel = num_samples;
		multiJittered* sampler_ptr = new multiJittered(num_samples);
		occluder_ptr->set_sampler(sampler_ptr);

		cam.initialize();
		cam.parallel_render(world, *occluder_ptr);
		SDL_RenderPresent(renderer);

		string path = "../../screenshot/sphereMultiJittered/num_samples-" + to_string(num_samples) + ".bmp";
		saveScreenshotBMP(path);
	}
}

void sphereRandom(camera cam, hittable_list world) {

	//Luce ambient occluder
	ambient_occluder* occluder_ptr = new ambient_occluder(getColor("white"), getColor("white"), getColor("white"), 1.0);
	occluder_ptr->set_min_amount(0.0);

	//Cube base
	mesh* cube = new mesh("../models/cube.obj", "../models/");
	texture* colore_cubo = new image_texture("../models/grigio.jpg");
	material* m_cube = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_cubo = make_shared<instance>(cube, m_cube);
	instance_ptr_cubo->scale(40.0, 1.0, 40.0);
	instance_ptr_cubo->translate(0.0f, -0.5f, 0.0f);
	m_cube->texture = colore_cubo;
	world.add(instance_ptr_cubo);

	//Sfera principale 
	mesh* sfera_obj = new mesh("../models/sfera.obj", "../models/");
	texture* colore_sfera_obj = new image_texture("../models/rosso.jpg");
	material* m_sfera_obj = new material(getColor("red"), getColor("red"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_sfera_obj = make_shared<instance>(sfera_obj, m_sfera_obj);
	instance_ptr_sfera_obj->scale(0.3, 0.3, 0.3);
	m_sfera_obj->texture = colore_sfera_obj;
	world.add(instance_ptr_sfera_obj);

	//Camera
	cam.lookfrom = point3(0, 5, 10);
	cam.lookat = point3(0, 0.5, 0);

	int num_samples = 0;

	std::vector<int> vec_samples = { 1, 16, 64, 256 };

	for (int i = 0; i < vec_samples.size(); i++) {
		//Sampler setting
		num_samples = vec_samples[i];
		cam.samples_per_pixel = num_samples;
		pure_random* sampler_ptr = new pure_random(num_samples);
		occluder_ptr->set_sampler(sampler_ptr);

		cam.initialize();
		cam.parallel_render(world, *occluder_ptr);
		SDL_RenderPresent(renderer);

		string path = "../../screenshot/sphereRandom/num_samples-" + to_string(num_samples) + ".bmp";
		saveScreenshotBMP(path);
	}
}

void sphereRegular(camera cam, hittable_list world) {

	//Luce ambient occluder
	ambient_occluder* occluder_ptr = new ambient_occluder(getColor("white"), getColor("white"), getColor("white"), 1.0);
	occluder_ptr->set_min_amount(0.0);

	//Cube base
	mesh* cube = new mesh("../models/cube.obj", "../models/");
	texture* colore_cubo = new image_texture("../models/grigio.jpg");
	material* m_cube = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_cubo = make_shared<instance>(cube, m_cube);
	instance_ptr_cubo->scale(40.0, 1.0, 40.0);
	instance_ptr_cubo->translate(0.0f, -0.5f, 0.0f);
	m_cube->texture = colore_cubo;
	world.add(instance_ptr_cubo);

	//Sfera principale 
	mesh* sfera_obj = new mesh("../models/sfera.obj", "../models/");
	texture* colore_sfera_obj = new image_texture("../models/rosso.jpg");
	material* m_sfera_obj = new material(getColor("red"), getColor("red"), getColor("red"), 0.8f, 0.0f);
	auto instance_ptr_sfera_obj = make_shared<instance>(sfera_obj, m_sfera_obj);
	instance_ptr_sfera_obj->scale(0.3, 0.3, 0.3);
	m_sfera_obj->texture = colore_sfera_obj;
	world.add(instance_ptr_sfera_obj);

	//Camera
	cam.lookfrom = point3(0, 5, 10);
	cam.lookat = point3(0, 0.5, 0);

	int num_samples = 0;

	std::vector<int> vec_samples = { 1, 16, 64, 256};

	for (int i = 0; i < vec_samples.size(); i++) {
		//Sampler setting
		num_samples = vec_samples[i];
		cam.samples_per_pixel = num_samples;
		regular* sampler_ptr = new regular(num_samples);
		occluder_ptr->set_sampler(sampler_ptr);

		cam.initialize();
		cam.parallel_render(world, *occluder_ptr);
		SDL_RenderPresent(renderer);

		string path = "../../screenshot/sphereRegular/num_samples-" + to_string(num_samples) + ".bmp";
		saveScreenshotBMP(path);
	}
}

void davidScene(camera cam, hittable_list world) {

	//Colori texture
	texture* verde  = new image_texture("../models/verde.jpg");
	texture* giallo = new image_texture("../models/giallo.jpg");
	texture* rosso  = new image_texture("../models/rosso.jpg");
	texture* blu    = new image_texture("../models/blu.jpg");
	texture* grigio = new image_texture("../models/grigio.jpg");
	texture* bianco = new image_texture("../models/bianco.jpg");
	texture* marmo = new image_texture("../models/marble.jpg");
	texture* scacchiera = new image_texture("../models/floor.jpg");

	//Sampler
	int num_samples = 64;
	float min_amount = 0.25f;
	multiJittered* sampler_ptr = new multiJittered(num_samples);

	//Luce ambient occluder
	ambient_occluder* occluder_ptr = new ambient_occluder(getColor("white"), getColor("white"), getColor("white"), 1.0);
	occluder_ptr->set_min_amount(min_amount);
	occluder_ptr->set_sampler(sampler_ptr);

	//Cube pavimento
	mesh* cube = new mesh("../models/cube.obj", "../models/");
	material* m_cube = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	m_cube->texture = bianco;
	auto instance_ptr_cubo = make_shared<instance>(cube, m_cube);
	instance_ptr_cubo->scale(40.0, 1.0, 40.0);
	instance_ptr_cubo->translate(0.0f, -0.5f, 0.0f);	
	world.add(instance_ptr_cubo);

	//Cube muro1
	material* m_cube2 = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_cubo2 = make_shared<instance>(cube, m_cube2);
	instance_ptr_cubo2->scale(0.1, 3.0, 5.0);
	instance_ptr_cubo2->translate(-2.3f, 1.5f, -0.5f);
	m_cube2->texture = bianco;
	world.add(instance_ptr_cubo2);

	//Cube muro3
	material* m_cube3 = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_cubo3 = make_shared<instance>(cube, m_cube3);
	instance_ptr_cubo3->scale(0.1, 3.0, 4.6);
	instance_ptr_cubo3->translate(3.0f, 1.5f, 0.0f);
	instance_ptr_cubo3->rotate_y(90.0f);
	m_cube3->texture = bianco;
	world.add(instance_ptr_cubo3);

	//Busto del David 
	mesh* david = new mesh("../models/david5perc.obj", "../models/");
	material* m_david = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_david = make_shared<instance>(david, m_david);
	instance_ptr_david->scale(0.007, 0.007, 0.007);
	//instance_ptr_david->rotate_y(45.0f);
	instance_ptr_david->translate(-1.6f, 0.0f, -1.5f);
	m_david->texture = bianco;
	world.add(instance_ptr_david);

	cam.lookfrom = point3(1, 2, 3.5);
	cam.lookat = point3(-0.5, 1.2, 0);

	cam.samples_per_pixel = num_samples;
	cam.initialize();

	cam.parallel_render(world, *occluder_ptr);
	SDL_RenderPresent(renderer);
	string path = "../../screenshot/david_scene/num_samples-" + to_string(num_samples) +"-min_amount-"+ to_string(min_amount) + ".bmp";
	saveScreenshotBMP(path);
}

void city(camera cam, hittable_list world) {

	//Colori texture
	texture* verde  = new image_texture("../models/verde.jpg");
	texture* giallo = new image_texture("../models/giallo.jpg");
	texture* rosso  = new image_texture("../models/rosso.jpg");
	texture* blu    = new image_texture("../models/blu.jpg");
	texture* grigio = new image_texture("../models/grigio.jpg");
	texture* bianco = new image_texture("../models/bianco.jpg");

	//Sampler
	int num_samples = 256; // 4 64 256 
	float min_amount = 0.0f;// 0.70 0.35 0.0
	multiJittered* sampler_ptr = new multiJittered(num_samples);

	//Luce ambient occluder
	ambient_occluder* occluder_ptr = new ambient_occluder(getColor("darkgray"), getColor("lightgray"), getColor("black"), 1.0);
	occluder_ptr->set_min_amount(min_amount);
	occluder_ptr->set_sampler(sampler_ptr);

	////Luce point_light
	//point3 light_position(-2.0f, 20, -20.0f);
	//point_light* worldlight = new point_light(light_position, getColor("darkgray"), getColor("lightgray"), getColor("black"));
	//world.addLight(worldlight);

	//Cube base
	mesh* cube = new mesh("../models/cube.obj", "../models/");
	material* m_cube = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_cubo = make_shared<instance>(cube, m_cube);
	instance_ptr_cubo->scale(60.0, 1.0, 60.0);
	instance_ptr_cubo->translate(0.0f, -0.5f, 0.0f);
	m_cube->texture = bianco;
	world.add(instance_ptr_cubo);

	srand(10);

	auto instance_ptr = make_shared<instance>(cube, new material());

	for (int a = -11; a < 5; a++) {
		for (int b = -11; b < 1; b++) {
			point3 center(a + 0.9f * random_float(), 0.1f, b + 0.9f * random_float());
			material* m_randomCube = new material();
			instance_ptr = make_shared<instance>(cube, m_randomCube);
			instance_ptr->scale(0.2f, 0.2f, 0.2f);
			instance_ptr->translate(center[0], center[1], center[2]);
			m_randomCube->texture = blu;
			world.add(instance_ptr);
		}
	}

	for (int a = -11; a < 5; a++) {
		for (int b = -11; b < 0; b++) {
			point3 center(a + 0.9f * random_float(), 0.5f, b + 0.9f * random_float());
			material* m_randomCube = new material();
			instance_ptr = make_shared<instance>(cube, m_randomCube);
			instance_ptr->scale(0.4f, 1.0f, 0.2f);
			instance_ptr->translate(center[0], center[1], center[2]);
			m_randomCube->texture = blu;
			world.add(instance_ptr);
		}
	}

	for (int a = -11; a < 5; a++) {
		for (int b = -11; b < -7; b++) {
			point3 center(a + 0.9f * random_float(), 1.0f, b + 0.9f * random_float());
			material* m_randomCube = new material();
			instance_ptr = make_shared<instance>(cube, m_randomCube);
			instance_ptr->scale(0.5f, 2.0f, 0.5f);
			instance_ptr->translate(center[0], center[1], center[2]);
			m_randomCube->texture = blu;
			world.add(instance_ptr);
		}
	}

	cam.lookfrom = point3(-20, 15, 15);
	cam.lookat = point3(-3, 0, -5);

	cam.samples_per_pixel = num_samples;
	cam.initialize();

	cam.parallel_render(world, *occluder_ptr);
	SDL_RenderPresent(renderer);
	string path = "../../screenshot/city/num_samples-" + to_string(num_samples) + "-min_amount-" + to_string(min_amount) + ".bmp";
	saveScreenshotBMP(path);

}

void pointLight(camera cam, hittable_list world) {

	//Colori texture
	texture* verde  = new image_texture("../models/verde.jpg");
	texture* giallo = new image_texture("../models/giallo.jpg");
	texture* rosso  = new image_texture("../models/rosso.jpg");
	texture* blu    = new image_texture("../models/blu.jpg");
	texture* grigio = new image_texture("../models/grigio.jpg");
	texture* bianco = new image_texture("../models/grigio.jpg");

	//Sampler
	int num_samples = 256;
	float min_amount = 0.25f;
	multiJittered* sampler_ptr = new multiJittered(num_samples);

	//Luce ambient occluder
	ambient_occluder* occluder_ptr = new ambient_occluder(getColor("darkgray"), getColor("lightgray"), getColor("black"), 1.0);
	occluder_ptr->set_min_amount(min_amount);
	occluder_ptr->set_sampler(sampler_ptr);

	//Luce point_light
	point3 light_position(-1.0f, 4, 4.0f);
	point_light* pointLight = new point_light(light_position, getColor("darkgray"), getColor("lightgray"), getColor("black"));
	//world.addLight(pointLight);

	//Cube base
	mesh* cube = new mesh("../models/cube.obj", "../models/");
	material* m_cube = new material(getColor("white"), getColor("white"), getColor("white"), 1.0f, 0.0f);
	texture* pavimento_texture = new image_texture("../models/Floor_Paint.png");
	auto instance_ptr_cubo = make_shared<instance>(cube, m_cube);
	instance_ptr_cubo->scale(40.0, 1.0, 40.0);
	instance_ptr_cubo->translate(0.0f, -0.5f, 0.0f);
	m_cube->texture = pavimento_texture;
	world.add(instance_ptr_cubo);
	
	//Cube muro1
	material* m_cube3 = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	texture* muro_texture = new image_texture("../models/South_Wall_Paint.png");
	auto instance_ptr_cubo3 = make_shared<instance>(cube, m_cube3);
	instance_ptr_cubo3->scale(0.1, 3.0, 5.0);
	instance_ptr_cubo3->translate(-2.3f, 1.5f, -0.5f);
	m_cube3->texture = muro_texture;
	world.add(instance_ptr_cubo3);

	//Cube muro2
	material* m_cube4 = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_cubo4 = make_shared<instance>(cube, m_cube4);
	instance_ptr_cubo4->scale(0.1, 3.0, 5.0);
	instance_ptr_cubo4->translate(2.3f, 1.5f, -0.5f);
	m_cube4->texture = muro_texture;
	world.add(instance_ptr_cubo4);

	//Cube muro3
	material* m_cube5 = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_cubo5 = make_shared<instance>(cube, m_cube5);
	instance_ptr_cubo5->scale(0.1, 3.0, 4.6);
	instance_ptr_cubo5->translate(3.0f, 1.5f, 0.0f);
	instance_ptr_cubo5->rotate_y(90.0f);
	m_cube5->texture = muro_texture;
	world.add(instance_ptr_cubo5);

	//Buzz
	mesh* buzz_obj = new mesh("../models/buzz.obj", "../models/");
	texture* buzz_tex = new image_texture("../models/buzz_texture.jpg");
	material* m_buzz_obj = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_buzz_obj = make_shared<instance>(buzz_obj, m_buzz_obj);
	instance_ptr_buzz_obj->scale(0.05, 0.05, 0.05);
	instance_ptr_buzz_obj->rotate_y(-90.0f);
	instance_ptr_buzz_obj->translate(1.0f, 0.0f, 1.0f);
	m_buzz_obj->texture = buzz_tex;
	world.add(instance_ptr_buzz_obj);

	//Woody
	mesh* woody_obj = new mesh("../models/woody.obj", "../models/");
	texture* woody_tex = new image_texture("../models/woody_texture.jpg");
	material* m_woody_obj = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_woody_obj = make_shared<instance>(woody_obj, m_woody_obj);
	instance_ptr_woody_obj->scale(0.05, 0.05, 0.05);
	instance_ptr_woody_obj->rotate_y(90.0f);
	instance_ptr_woody_obj->translate(-0.7f, 0.0f, 1.0f);
	m_woody_obj->texture = woody_tex;
	world.add(instance_ptr_woody_obj);

	//Palla obj
	mesh* sfera_obj = new mesh("../models/ball.obj", "../models/");
	texture* balltex = new image_texture("../models/pixar_ball.png");
	material* m_sfera_obj = new material(getColor("white"), getColor("white"), getColor("white"), 0.8f, 0.0f);
	auto instance_ptr_sfera_obj = make_shared<instance>(sfera_obj, m_sfera_obj);
	instance_ptr_sfera_obj->scale(0.007, 0.007, 0.007);
	instance_ptr_sfera_obj->translate(0.0f, 0.5f, 1.0f);
	m_sfera_obj->texture = balltex;
	world.add(instance_ptr_sfera_obj);

	cam.lookfrom = point3(0, 2, 9.0f);
	cam.lookat = point3(0, 1.1, 0);

	cam.samples_per_pixel = num_samples;
	cam.initialize();
	
	cam.parallel_render(world,*occluder_ptr); // Con AO
	SDL_RenderPresent(renderer);
	string path = "../../screenshot/pointLight/num_samples-" + to_string(num_samples) + "-min_amount-" + to_string(min_amount) + ".bmp";
	saveScreenshotBMP(path);
}

void printMenu() {
	cout << '\n';
	cout << "+-----------------MENU-------------------+" << endl;
	cout << "+ a. Multijittered sampling sphere-plane +" << endl;
	cout << "+ b. Regular sampling sphere-plane       +" << endl;
	cout << "+ c. Random sampling sphere-plane        +" << endl;
	cout << "+ d. David scene render                  +" << endl;
	cout << "+ e. Point light scene                   +" << endl;
	cout << "+ f. Ambient occlusion city scene        +" << endl;
	cout << "+----------------------------------------+" << endl;
}

int main(int argc, char* argv[])
{
	hittable_list world;
	camera cam;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 1280; 
	cam.samples_per_pixel = 4;
	cam.vfov = 20;

	printMenu();

	cam.initialize();
	if (init(cam.image_width, cam.image_height) == 1) {
		cout << "App Error! " << std::endl;
		return 1;
	}

	SDL_Event event;
	bool quit = false;
	double dif;

	while (SDL_PollEvent(&event) || (!quit)) {

		switch (event.type) {

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;


			case SDLK_a:
				sphereMultiJittered(cam, world);
				printMenu();
				break;

			case SDLK_b:
				sphereRegular(cam, world);
				printMenu();
				break;

			case SDLK_c:
				sphereRandom(cam, world);
				printMenu();
				break;

			case SDLK_d:
				time_t start, end;
				time(&start);
				davidScene(cam, world);
				time(&end);
				dif = difftime(end, start);
				cout << "\n" << "Rendering time: " << dif << "\n";
				printMenu();
				break;

			case SDLK_e:
				start, end;
				time(&start);
				pointLight(cam, world);
				time(&end);
				dif = difftime(end, start);
				cout << "\n" << "Rendering time: " << dif << "\n";
				printMenu();
				break;

			case SDLK_f:
				start, end;
				time(&start);
				city(cam, world);
				time(&end);
				dif = difftime(end, start);
				cout << "\n" << "Rendering time: " << dif << "\n";
				printMenu();
				break;

			}

		}
	}

	close();
	return 0;

}

