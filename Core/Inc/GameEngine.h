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

  void m_checkPeripherals();
  
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
  DigitalIn* keys[4];

  /**
   * @brief Buzzer
   */
  PwmOut* buzzer;

  /**
   * @brief 2 joysticks L, R
   */
  Joystick* joystics[2];

  BusOut* led_bgr;

  // Systems
  ECS::EntitySystem *m_TransformSystem;
  ECS::EntitySystem *m_RenderSystem;
  ECS::EntitySystem *m_UIControlSystem;
  ECS::EntitySystem *m_GameControlSystem;
  ECS::EntitySystem *m_PeripheralCheckSystem_UI;


 private:
  void m_initPeripherals();

  void m_freePeripherals() const;

  void m_initEarlyData();

  /**
   * @brief Register Systems to Manager. Systems are executed by the order of registration.
   */
  void m_registerSystems();

  void m_makeMainMenu();

  void m_makeSelectionMenu();

  // Game levels
  void m_makeGameLevel(int level);
};
}  // namespace Engine

#endif
