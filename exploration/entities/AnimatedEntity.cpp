#include "AnimatedEntity.h"

AnimatedEntity::AnimatedEntity(Model* model, const EntitySpawnInfo& info, IAnimator* animator)
  : Entity{ model, info }
  , animator{ animator }
{ }

void AnimatedEntity::draw_faces(GameState& state, Program& program, float time)
{
  animator->applyAnimation(program, time, model->joints);
  program.setFloat("draw_percentage", 1.0f);
  model->draw_faces(program, time, position, rotation, scale);
}

void AnimatedEntity::draw_lines(GameState& state, Program& program, float time)
{
  animator->applyAnimation(program, time, model->joints);
  program.setFloat("draw_percentage", 1.0f);
  model->draw_lines(program, time, position, rotation, scale);
}

void AnimatedEntity::draw_debug(GameState& state, Program& program, float time)
{

}
