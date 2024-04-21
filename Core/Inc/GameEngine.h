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
#include "ECS.h"
#include "N5110.h"
#include "Joystick.h"
#include "singleton.h"

namespace Engine {
class GameManager {
 private:
  GameManager() {};
 public:
 SINGLETON(GameManager);

  void Init();

  ~GameManager();
  
  /**
   * @brief ECS world to manage all the entities and components.
   */
  ECS::World* ecs;

  /**
   *  @brief LCD display
   */
  N5110* lcd;

  /**
   *  @brief 4 keys, A B C D
   */
  InterruptIn* keys[4];

  /**
   * @brief Buzzer
   */
  PwmOut* buzzer;

  /**
   * @brief 2 joysticks L, R
   */
  Joystick* joystics[2];

  BusOut* led_bgr;

 private:
  void m_initPeripherals();

  void m_freePeripherals() const;

  void m_initEarlyData();

  void m_registerSystems();

  ECS::Entity* m_makeMainMenu();
};
}  // namespace Engine

#endif
