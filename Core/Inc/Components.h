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
/**
 * @class Transform
 * @brief Class for managing position, rotation, and scale of an object.
 */
class Transform {
 public:
  /**
   * @brief Array of 3 float values representing the position of the object.
   */
  float Position[3];

  /**
   * @brief Array of 3 float values representing the rotation of the object.
   */
  float Rotation[3];

  /**
   * @brief Array of 3 float values representing the scale of the object.
   */
  float Scale[3];
};

/**
 * @class Player
 * @brief Class for managing player level and experience.
 */
class Player {
 public:
  /**
   * @brief Level of the player.
   */
  int8_t Level;

  /**
   * @brief Experience of the player.
   */
  int8_t Exp;
};

/**
 * @class Note
 * @brief Class for managing note type and score.
 */
class Note {
 public:
  /**
   * @brief Type of the note.
   */
  int8_t Type;

  /**
   * @brief Score of the note.
   */
  int8_t Score;
};

class Render {
 public:
  /**
   * @brief A 2D array of bool values representing the sprite data.
   */
  bool ** Data;

  /**
   * @brief The left-up point's x of the sprite.
   */
  int8_t x;

  /**
   * @brief The left-up point's y of the sprite.
   */
  int8_t y;
};
}
#endif //XJEL2645NEWVERSION_CORE_SRC_COMPONENTS_H_
