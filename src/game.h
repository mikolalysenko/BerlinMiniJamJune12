#ifndef GAME_H
#define GAME_H

#include <array>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace Game {
  void init();
  void input(std::array<Eigen::Vector2f, 2> const& mouse_state);
  void tick(double dt);
  void draw();
  void hud();
};


#endif

