/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Events.h
 * Author: Yuanzhen Gan
 * Date: 24-5-1
 * Modify Record:
 */

#ifndef XJEL2645NEWVERSION_CORE_SRC_EVENTS_H_
#define XJEL2645NEWVERSION_CORE_SRC_EVENTS_H_

#include "ECS.h"
#include "Joystick.h"

namespace Events {
class JoystickUpdateEvent {
 ECS_DECLARE_TYPE;

  int8_t id;

  Direction direction;

  float magnitude;

  float angle;

  Vector2D coordinate;

  Vector2D mapped_coord;

  Polar polar;
};

class KeypressEvent {
 ECS_DECLARE_TYPE;
  int8_t id;
};
};

#endif //XJEL2645NEWVERSION_CORE_SRC_EVENTS_H_
