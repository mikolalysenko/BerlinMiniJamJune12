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
#include "obstacle.h"
#include "level.h"

using namespace std;
using namespace Eigen;

namespace Game {

vector<Level> game_levels;

Level transition_level;

int current_level = 0;
double current_time = 0;

Vector3f player_position;
Vector3f player_velocity;
float player_move_speed = 8;
double score = 0.0f;

int next_life = 5000;
int life_points = 10000;
int lives = 5;

enum GAME_STATE {
  PLAYING,
  TRANSITION,
  TAKE_HIT,
  GAME_OVER,
};
GAME_STATE game_state = PLAYING;
double hit_time = 0;
double transition_time = 9;

int difficulty = 1;


vector<Obstacle>  obstacles;

Level& level() {
  if(game_state == TRANSITION) {
    return transition_level;
  }
  return game_levels[current_level];
}



void createObstacle(int type) {

  if(type == OBST_PILLAR) {
    Obstacle tmp;
    tmp.configuration.setIdentity();  
    tmp.configuration.scale(Vector3f(2, 2.2, 2));
    tmp.configuration.pretranslate(Vector3f(
      80 * (drand48() - 0.5), 
      0, 
      player_position[2] - 500));
    tmp.velocity = Vector3f(0,0,0);
    tmp.angular_velocity = Vector3f(0,0,0);
    tmp.solid = get_artwork("pillar");
    obstacles.push_back(tmp);
  }
  else if(type == OBST_TORUS) {
    Obstacle tmp;
    tmp.velocity = Vector3f::Random().normalized() * drand48();
    tmp.angular_velocity = Vector3f::Random().normalized() * drand48();
    tmp.configuration.setIdentity();
    tmp.configuration.translate(Vector3f(
      40 * (drand48() - 0.5), 
      24 * (drand48() - 0.5),
      player_position[2] - 500));
    tmp.solid = get_artwork("torus");
    obstacles.push_back(tmp);
  }
  else if(type == OBST_SHRUB) {
    Obstacle tmp;
    tmp.configuration.setIdentity();  
    tmp.configuration.scale(Vector3f(drand48() + 2.0, drand48() + 2.0, drand48() + 2.0));
    tmp.configuration.pretranslate(Vector3f(
      80 * (drand48() - 0.5), 
      -10 + drand48(), 
      player_position[2] - 500));
    tmp.velocity = Vector3f(0,0,0);
    tmp.angular_velocity = Vector3f(0,0,0);
    tmp.solid = get_artwork("shrub");
    obstacles.push_back(tmp);
  }
  else if(type == OBST_DONUT_TRAIN) {
  
    float h = 24 * (drand48() - 0.5);
  
    for(int i=0; i<=30; ++i) {
      Obstacle tmp;
      tmp.velocity = Vector3f(0, 0, 0);
      tmp.angular_velocity = Vector3f(1, 0, 0);
      tmp.configuration.setIdentity();
      tmp.configuration.rotate(AngleAxisf(i * M_PI/2.0, Vector3f(1, 0, 0)));
      tmp.configuration.pretranslate(Vector3f(
        2*i - 30, 
        h,
        player_position[2] - 250));
      tmp.solid = get_artwork("torus");
      obstacles.push_back(tmp);
    }
  }
  else if(type == OBST_BOXOID) {
    Obstacle tmp;
    tmp.velocity = Vector3f(0,0,0);
    tmp.angular_velocity = Vector3f(0, 0, 0);
    tmp.configuration.setIdentity();
    tmp.configuration.scale(4 * Vector3f(1,0.7,1));
    tmp.configuration.pretranslate(Vector3f(
      5 * drand48() - 2.5,
      5 * drand48() - 2.5,
      player_position[2] - 400));
    tmp.solid = get_artwork("boxoid");
    obstacles.push_back(tmp);
  }
  else if(type == OBST_SPIKE) {
    Obstacle tmp;
    tmp.configuration.setIdentity();  
    tmp.configuration.scale(Vector3f(2, 2, 2));
    tmp.configuration.translate(Vector3f(
      40 * (drand48() - 0.5), 
      20 * (drand48() - 0.5), 
      player_position[2] - 500));
    tmp.velocity = Vector3f(0,0,0);
    tmp.angular_velocity = Vector3f(0,0,0);
    tmp.solid = get_artwork("spike");
    obstacles.push_back(tmp);
  }
  else if(type == OBST_SPIKE_RING) {
  
  
    float r = drand48() * M_PI * 2;
  
    for(float theta=0; theta<=2*M_PI; theta += M_PI/16) {
      Obstacle tmp;
      
      tmp.velocity = Vector3f(
        cos(theta + r), 
        sin(theta + r), 0) * 50;
        
      tmp.configuration.setIdentity();  
      //tmp.configuration.scale(Vector3f(2, 2, 2));
      tmp.configuration.pretranslate(Vector3f(
        50 * cos(theta), 
        50 * sin(theta), 
        player_position[2] - 250));
        
      tmp.angular_velocity = Vector3f(0,0,0);
      tmp.solid = get_artwork("spike");
      obstacles.push_back(tmp);
    }

  }
  
  
}


void init() {
  game_levels = get_levels();
  transition_level = {
    1, 
    0,
    0,
    { Vector3f(1, 1, 1), Vector3f(1,1,1) },
    Vector3f(1,1,1),
    {},
  };
}

void input(std::array<Eigen::Vector2f, 2> const& mouse_state) {

  float dx = 0.5 - mouse_state[1][0],
        dy = mouse_state[1][1] - 0.5;

  player_velocity[0] = -0.01 * level().fly_speed * (pow(player_move_speed * (0.5 - mouse_state[1][0]), 3) + 100*dx);
  player_velocity[1] = -0.01 * level().fly_speed * (pow(player_move_speed * (mouse_state[1][1] - 0.5), 3) + 100*dy);
  player_velocity[2] = -level().fly_speed;
  
  if(glfwGetKey('S') == GLFW_PRESS && game_state == PLAYING) {
    cout << "Skipping level" << endl;
    current_time += 10000;
  }
}

bool checkBounds(Vector3f const& position) {
  return position[2] > player_position[2] + 100;
}

void tick(double dt) {

  dt *= difficulty;


  if(game_state == TAKE_HIT || game_state == GAME_OVER) {
    player_velocity = Vector3f(0, -8, 0);
  }

  player_position += player_velocity * dt;


  if(player_position[0] < -20) {
    player_position[0] = -20;
  }
  else if(player_position[0] > 20) {
    player_position[0] = 20;
  }
  
  if(player_position[1] < -10) {
    player_position[1] = -10;
  }
  else if(player_position[1] > 12) {
    player_position[1] = 12;
  }


  if(game_state == TAKE_HIT) {  
    hit_time -= dt;
    if(hit_time <= 0) {
      game_state = PLAYING;
    }
  } else if(game_state == GAME_OVER) {
  
    //Game over
  
  } else {

    if(game_state == PLAYING) {
      current_time += dt;
      if(current_time > level().duration) {
        current_time = 0;
        current_level = (current_level + 1) % game_levels.size();
        transition_time = 1;
        game_state = TRANSITION;
        
        if(current_level == 0) {
          ++difficulty;
        }
      }
    }
    
    
    if(game_state == TRANSITION) {
      transition_time -= dt;
      if(transition_time <= 0) {
        game_state = PLAYING;
      }
      else {
        auto p = game_levels[(current_level+game_levels.size()-1)%game_levels.size()];
        auto n = game_levels[current_level];
        float t = transition_time, ti = 1.0f - transition_time;
        transition_level.fly_speed = t * p.fly_speed + ti * n.fly_speed;
        transition_level.score_multiplier = n.score_multiplier;
        for(int i=0; i<2; ++i) {
          transition_level.tile_colors[i] = t * p.tile_colors[i] + ti * n.tile_colors[i];
        }
        transition_level.background_color = t * p.background_color + ti * n.background_color;
      }
    }
    
    
    for(int i=obstacles.size()-1; i>=0; --i) {
      obstacles[i].tick(dt);
      
      if(checkBounds(obstacles[i].configuration.translation())) {
        obstacles[i] = obstacles[obstacles.size()-1];
        obstacles.pop_back();
        continue;
      }
      
      if(obstacles[i].checkCollision(player_position)) {
        obstacles[i] = obstacles[obstacles.size()-1];
        obstacles.pop_back();
        
        if(--lives < 0) {
          game_state = GAME_OVER;
        }
        else {
          game_state = TAKE_HIT;
          hit_time = 3;
        }        
      }
    }
    
    
    auto oblist = level().obstacles;
    for(int i=0; i<oblist.size(); ++i) {
      if(drand48() < oblist[i].first * dt) {
        createObstacle(oblist[i].second);
      }
    }
    
    score += dt * level().score_multiplier;
    
    //Gain lives
    while(score > next_life) {
      next_life += life_points;
      lives++;
    }
  }

}


void enable_lights() {
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);	
}

void disable_lights() {
	glDisable(GL_NORMALIZE);	
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
}

void setup_lights() {
	//Set up material
	GLfloat specular[] = {0.9f, 0.9f, 0.9f, 0.9f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular );
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.8f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);	
	glShadeModel(GL_SMOOTH);
		
	Vector3f lp(0, 1000, 1000);
	
	lp += player_position;
	
	GLfloat light_pos[4];
	light_pos[0] = lp[0];
	light_pos[1] = lp[1];
	light_pos[2] = lp[2];
	light_pos[3] = 1.f;
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}


void draw() {
  
  
  auto bg_color = level().background_color;
  if(game_state == TAKE_HIT || game_state == GAME_OVER) {
    bg_color = Vector3f(drand48() + 0.5, drand48() + 0.5, 0.5);
  }

  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_START, 300.0);
  glFogf(GL_FOG_END, 450);
  glFogfv(GL_FOG_COLOR, bg_color.data());
  
  glClearColor(bg_color[0], bg_color[1], bg_color[2], 1);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glTranslatef(-player_position[0], -player_position[1], -player_position[2]);
  
  setup_lights();

  enable_lights();
  
  //Draw ground plane
  glBegin(GL_QUADS);
  
  Eigen::Vector3f origin((int)(player_position[0] / 20) * 20 - 300, -13, (int)(player_position[2] / 20) * 20);
  for(int i=0; i<60; ++i)
  for(int j=0; j<80; ++j) {
    int parity = (i+j) & 1;
    
    glColor3f(level().tile_colors[parity][0],
      level().tile_colors[parity][1],
      level().tile_colors[parity][2]);
    glNormal3f(0, 1, 0);
    
    Vector3f p = origin + 10 * Vector3f(i, 0, -j);
    
    glVertex3f(p[0], p[1], p[2]);
    glVertex3f(p[0]+10, p[1], p[2]);
    glVertex3f(p[0]+10, p[1], p[2]+10);
    glVertex3f(p[0], p[1], p[2]+10);
  }
  glEnd();
   
  //Draw obstacles
  for(int i=0; i<obstacles.size(); ++i) {
    obstacles[i].draw();
  }
  
  disable_lights();
}

void hud() {

  if(game_state == GAME_OVER) {
    float c = 0.8 * drand48();
    glColor3f(c,c,c);
    show_text("GAME OVER", 0.35, 0.5, 0.1, TEXT_STYLE_BOLD);
    
    stringstream ss;
    ss << "Final Score: " << (long long) score;
    show_text(ss.str().c_str(), 0.4, 0.4, 0.05, TEXT_STYLE_BOLD);
  }  
  else {

    //Draw HUD
    stringstream ss;
    ss << "Score: " << (long long)(score);
    show_text(ss.str().c_str(), 0, 0);
    
    stringstream s2;
    s2 << "Lives: " << lives;
    show_text(s2.str().c_str(), 0.9, 0);
    
  
  }
}

};

