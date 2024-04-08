/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: GameEngine.h
 * Author: Yuanzhen Gan
 * Date: 2024-04-01
 * Modify Record:
 */

#ifndef __GAME_ENGINE_H_
#define __GAME_ENGINE_H_

#include "matrix.h"
#include "GlobalDefines.h"

namespace Engine {
class GameManager {
 public:
  GameManager();
  ~GameManager();
  
  /**
   * @brief ECS world to manage all the entities and components.
   */
  ecs_t *ecs;

 private:
  void m_registerComponents();
};
}  // namespace Engine

#endif
