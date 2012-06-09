#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include "solid.h"

struct Obstacle {
  Eigen::Affine3f configuration;
  Eigen::Vector3f velocity, angular_velocity;
  Solid* solid;

  void tick(double delta_t);  
  void draw();
  
  bool checkCollision(Eigen::Vector3f const& x) const {
  	auto tinv = configuration.inverse();
	  return (*solid)(tinv * x) > 0;
  }
  
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
};

#endif

