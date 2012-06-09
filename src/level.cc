

#include "level.h"




using namespace std;
using namespace Eigen;

std::vector<Level> get_levels() {

  vector<Level> result;
  
  
  {
    Level level = {
      80,
      40,
      100,
      { Vector3f(1, 0, 0), Vector3f(1, 1, 1) },
      Vector3f(0.3, 0.4, 0.5),
      {
        {4, OBST_SHRUB},
        {0.6, OBST_TORUS},
        {0.5, OBST_PILLAR},
      }      
    };
    result.push_back(level);
  }


  {
    Level level = {
      120,
      45,
      200,
      { Vector3f(0.2, 0.4, 0.3), Vector3f(0.1, 0.3, 0.8) },
      Vector3f(0.01, 0.08, 0.1),
      {
        {1, OBST_BOXOID},
        {0.2, OBST_TORUS},
      },
    };
    result.push_back(level);
  }
 
  {
    Level level = {
      180,
      45,
      400,
      { Vector3f(0.7, 0.8, 0.3), Vector3f(0.2, 0.6, 0.1) },
      Vector3f(0.6, 0.7, 0.2),
      {
        {3, OBST_SHRUB},
        {2, OBST_TORUS},
        {0.8, OBST_PILLAR},
        {0.6, OBST_DONUT_TRAIN}
      }
    };
    result.push_back(level);
  }
 
   
  
  {
    Level level = {
      250,
      70,
      600,
      { Vector3f(0.9, 0.9, 0.3), Vector3f(0.5, 0.6, 0.8) },
      Vector3f(0.9, 0.8, 0.8),
      {
        {4, OBST_SPIKE},
        {2.5, OBST_SPIKE_RING},
      }
    };
    result.push_back(level);
  }

  
  {
    Level level = {
      500,
      5000,
      800,
      { Vector3f(0, 0, 0), Vector3f(1,1,1) },
      Vector3f(1, 0.5, 0.1),
      {
        {3.0, OBST_SHRUB},
        {5.0, OBST_TORUS},
        {5.0, OBST_SPIKE},
        {1.0, OBST_PILLAR},
        {0.01, OBST_BOXOID},
        {1.0, OBST_DONUT_TRAIN},
      }
    };
    result.push_back(level);
  }
  
  return result;
}
