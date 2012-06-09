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

namespace Game {

void init() {
}

void input(std::array<Eigen::Vector2f, 2> const& mouse_state) {
}

void tick(double dt) {
}

void draw() {
  //Draw the game
  auto torus = get_artwork("torus");
  

  cout << torus->mesh.vertices().size() << endl;
  
  //glTranslatef(0, -10, 0);
  
  glBegin(GL_POINTS);
  for(int i=-20; i<20; ++i)
  for(int j=-20; j<20; ++j)
  for(int k=-20; k<20; ++k) {
    glColor3f(i, j, k);
    glVertex3f(i, j, k);
  }
  glEnd();
  
  
  glTranslatef(0, 0, -20);
  torus->draw();
}

void hud() {
  //Draw HUD
}

};

