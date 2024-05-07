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
#include "ECS.h"
#include <tuple>
#include <functional>
#include <list>

namespace Components {

/**
 * @class Transform
 * @brief Class for managing position, rotation, and scale of an object.
 */
class Transform {
 public:
 ECS_DECLARE_TYPE;

  /**
    * Used to indicate which group this Component is attached.
    */
  int8_t GroupID;

  /**
   * @brief Array of 3 float values representing the position of the object.
   */
  std::tuple<float, float, float> Position;

  /**
   * @brief Array of 3 float values representing the rotation of the object.
   */
  std::tuple<float, float, float> Rotation;

  /**
   * @brief Array of 3 float values representing the scale of the object.
   */
  std::tuple<float, float, float> Scale;
};

/**
 * @class Player
 * @brief Class for managing player level and experience.
 */
class Player {
 ECS_DECLARE_TYPE;
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
 ECS_DECLARE_TYPE;
 public:
  /**
   * Used to indicate which group this Component is attached.
   */
  int8_t GroupID;

  /**
   * @brief Type of the note.
   */
  int8_t Type;

  /**
   * @brief Score of the note.
   */
  int8_t Score;

  /**
   * Falling speed of the note.
   */
  float Speed;

  bool Played = false;
};

class Render {
 ECS_DECLARE_TYPE;
 public:

  /**
   * Used to indicate which group this Component is attached.
   */
  int8_t GroupID;

  typedef enum Type_e {
    Spirit = 0,
    Text,
    Line,
    Rect,
    Circle
  } Type_t;

  union Data_ptr {
    bool *spirit_Data;
    char *text_Data;
    int8_t *line_Data;
    int8_t *rect_Data;
    int8_t *circle_Data;
  };

  Type_t Type;

  /**
   * Size of the font, 0 for 5x5 and 1 for 5x7
   */
  int8_t size = 0;

  /**
   *  @brief Whether the sprite is visible or not.
   */
  bool Visible;
  /**
   * @brief A 2D array of bool values representing the sprite data.
   */
  Data_ptr Data;

  /**
   * @brief The width (x) of the sprite.
   */
  int8_t x;

  /**
   * @brief The height (y) of the sprite.
   */
  int8_t y;

  bool Override = true;
};

/**
 * Store the states of UI selection
 */
class UIRender {
 ECS_DECLARE_TYPE;
 public:
  typedef struct UIComp {
    Transform trans;

    int8_t id;

    bool* spirit_Data = nullptr;

    int8_t x;

    int8_t y;

    std::function<void()> callback_functionA = []()->void{};
    std::function<void()> callback_functionB = []()->void{};
    std::function<void()> callback_functionC = []()->void{};
    std::function<void()> callback_functionD = []()->void{};
  } UIComp_t;

  int8_t selected = false;

  std::list<UIComp_t> m_comps;
};
}
#endif //XJEL2645NEWVERSION_CORE_SRC_COMPONENTS_H_
