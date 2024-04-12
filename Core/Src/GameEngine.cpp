/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: GameEngine.cpp   
 * Author: Yuanzhen Gan
 * Date: 2024-04-01
 * Modify Record:
 */

#include "GameEngine.h"
#include "Components.h"
#include "Systems.h"
#include "log.h"
#include "GlobalDefines.h"

Engine::GameManager::GameManager() {
  log_info("Initializing GameEngine...");
  log_info("AppName: %s", GAME_NAME);
  log_info("AppVersion: %s", VERSION_NUMBER);
  log_info("AppAuthor: %s", AUTHOR);

  m_initPeripherals();

  ecs = ecs_new(128, nullptr);
  m_registerComponents();
  m_registerSystems();
}
Engine::GameManager::~GameManager() {
  ecs_free(ecs);
  m_freePeripherals();
}
void Engine::GameManager::m_registerComponents() {
  log_info("Registering Components...");
  using namespace Components;
  TRANSFORM_COMP = ecs_register_component(ecs, sizeof(Transform), nullptr, nullptr);
  PLAYER_COMP = ecs_register_component(ecs, sizeof(Player), nullptr, nullptr);
  NOTE_COMP = ecs_register_component(ecs, sizeof(Note), nullptr, nullptr);
  RENDER_COMP = ecs_register_component(ecs, sizeof(Render), nullptr, nullptr);
}
void Engine::GameManager::m_registerSystems() {
  log_info("Registering Systems...");
  using namespace Systems;
  TRANSFORM_SYSTEM = ecs_register_system(ecs, TransformSystem, nullptr, nullptr, this);

  ecs_require_component(ecs, TRANSFORM_SYSTEM, Components::TRANSFORM_COMP);

  RENDER_SYSTEM = ecs_register_system(ecs, RenderSystem, nullptr, nullptr, this);

  ecs_require_component(ecs, RENDER_SYSTEM, Components::TRANSFORM_COMP);
  ecs_require_component(ecs, RENDER_SYSTEM, Components::RENDER_COMP);
}
void Engine::GameManager::m_initPeripherals() {
  log_info("Initializing Peripherals...");

  log_info("Initializing LCD...");
  lcd = new N5110(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);
  lcd->init(LPH7366_1);
  lcd->setContrast(0.5);
}
void Engine::GameManager::m_freePeripherals() const {
  delete lcd;
}


