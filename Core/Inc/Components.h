/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Components.h
 * Author: Yuanzhen Gan
 * Date: 2024-04-06
 * Modify Record:
 */
#ifndef XJEL2645NEWVERSION_CORE_SRC_COMPONENTS_H_
#define XJEL2645NEWVERSION_CORE_SRC_COMPONENTS_H_

#include "GlobalDefines.h"

namespace Components {
extern ecs_id_t TRANSFORM_COMP;
extern ecs_id_t PLAYER_COMP;
extern ecs_id_t NOTE_COMP;
extern ecs_id_t RENDER_COMP;
}

namespace Components {
  class Transform {
    public:
      float x;
      float y;
      float z;
  };
  class Player {
    public:
      float x;
      float y;
      float z;
  };
  class Note {
    public:
      float x;
      float y;
      float z;
  };
}
#endif //XJEL2645NEWVERSION_CORE_SRC_COMPONENTS_H_
