#ifndef WILT_INSTANCE_H
#define WILT_INSTANCE_H

#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "../GameState.h"

class IAnimator;
class Model;
class Program;

class Instance
{
public:
  Model* model;
  glm::vec3 position;
  float rotation;
  float scale;

  Instance(Model* model, glm::vec3 position, float rotation, float scale = 1.0f);

  virtual void update(GameState& state, float time);

  virtual void draw_faces(GameState& state, Program& program, float time);
  virtual void draw_lines(GameState& state, Program& program, float time);
  virtual void draw_debug(GameState& state, Program& program, float time);

}; // class Instance

#include "../graphics/model.h"
#include "../graphics/program.h"

#endif // !WILT_INSTANCE_H
