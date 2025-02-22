#ifndef WILT_FOLLOWCAMERA_H
#define WILT_FOLLOWCAMERA_H

#include "ICamera.h"

class Entity;

class FollowCamera : public ICamera
{
private:
  Entity** entity_;
  float desiredXAngle_;
  float desiredYAngle_;
  float desiredDistance_;
  glm::vec3 desiredPosition_; // world space
  glm::vec3 currentPosition_; // world space

public:
  FollowCamera(Entity** entity);

public:
  // ICamera overrides
  void update(GameState& state, float time) override;
  glm::mat4 getTransform() const override;
  glm::vec3 getPosition() const override;
  glm::vec3 getDirection() const override;
  float getAngle() const override;
};

#endif // !WILT_FOLLOWCAMERA_H
