#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <GL/glfw.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <mesh/mesh.h>

#include "solid.h"
#include "sound.h"
#include "text.h"
#include "assets.h"
#include "game.h"

using namespace std;
using namespace Eigen;
using namespace Mesh;

namespace App {

array<Eigen::Vector2f,2> mouse_state;
double fov=45., znear=1., zfar=1000.;
bool running = true;

void init() {
	srand(time(NULL));
	
	initialize_text();
	initialize_assets();
	
	Game::init();
}

void input() {
  if(!glfwGetWindowParam(GLFW_OPENED) ||
    glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
    running = false;
    return;
  }
  
  //Input scale factor
  int w, h;
  glfwGetWindowSize(&w, &h);
  float s = w;
  
  //Update mouse input
  int mx, my;
  glfwGetMousePos(&mx, &my);
  mouse_state[0] = mouse_state[1];    
  mouse_state[1][0] = mx;
  mouse_state[1][1] = my;
  
  Game::input(mouse_state);
}

void tick() {

	static double last_t = 0.0;
	auto t = glfwGetTime();
	auto dt = t - last_t;
	last_t = t;

  //Do tick
	Game::tick(dt);
}

void draw() {

	int w, h;
	glfwGetWindowSize(&w, &h);
	glViewport(0, 0, w, h);
	
	glClearColor(0.3, 0.4, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	//Set up projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (float)w / (float)h, znear, zfar);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  Game::draw();

	//text rendering should be the last thing we do in the render loop
	start_text_rendering();
	
	//draw text as white
	glColor3f(1, 1, 1);
	
	//Draw text
	Game::hud();
	
	end_text_rendering();
}

};


int initialize_libs()
{
  glfwInit();
  if (!glfwOpenWindow(640,480, 8, 8, 8, 8, 16, 0, GLFW_WINDOW))
    return 0;

	if(!initialize_sound_driver())
		return 0;
	
	return 1;
}


int main(int argc, char* argv[])
{
	if(!initialize_libs())
	{
		glfwTerminate();
		return -1;
	}
	
  glfwSetWindowTitle("asdf");

  App::init();
  while(App::running) {
    App::input();
    App::draw();
    App::tick();
    glfwSwapBuffers();
  }

  glfwTerminate();
  return 0;
}

