#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "solid.h"
#include "obstacle.h"

using namespace std;
using namespace Eigen;

void Obstacle::tick(double dt) {
  float l = angular_velocity.norm();
  if( l > 0.001 ) {
    configuration.rotate(AngleAxisf(l * dt, angular_velocity / l));
  }
  configuration.translate(dt * velocity);
}

void Obstacle::draw() {
  glPushMatrix();
	glMultMatrixf(configuration.data());
  solid->draw();
  glPopMatrix();
}

