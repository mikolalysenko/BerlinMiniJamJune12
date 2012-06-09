#ifndef LEVEL_H
#define LEVEL_H

#include <Eigen/Core>
#include <map>
#include <vector>

enum OBSTACLE_TYPES {
  
  OBST_PILLAR,
  OBST_TORUS,
  OBST_SHRUB,
  OBST_DONUT_TRAIN,
  OBST_BOXOID,
  OBST_SPIKE,
  OBST_SPIKE_RING,

  NUM_OBSTACLES,

};


struct Level {
  float fly_speed,
        duration,
        score_multiplier;
  Eigen::Vector3f tile_colors[2];
  Eigen::Vector3f background_color;
  
  std::vector< std::pair<float, int> > obstacles;
  
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
};

std::vector<Level> get_levels();


#endif


