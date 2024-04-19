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
#include <cstdint>
//#include "pico_ecs.h"
//
//volatile extern ecs_id_t TRANSFORM_COMP;
//volatile extern ecs_id_t PLAYER_COMP;
//volatile extern ecs_id_t NOTE_COMP;
//volatile extern ecs_id_t RENDER_COMP;

namespace Components {

//void m_initComponents(ecs_s *ecs);

/**
 * @class Transform
 * @brief Class for managing position, rotation, and scale of an object.
 */
struct Transform {
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
struct Player {
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
struct Note {
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

struct Render {
 public:
  /**
   *  @brief Whether the sprite is visible or not.
   */
  bool Visible;
  /**
   * @brief A 2D array of bool values representing the sprite data.
   */
  bool *Data;

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
