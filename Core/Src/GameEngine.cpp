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
#include "Spirits.h"

Engine::GameManager::GameManager() {
  log_info("Initializing GameEngine...");
  log_info("AppName: %s", GAME_NAME);
  log_info("AppVersion: %s", VERSION_NUMBER);
  log_info("AppAuthor: %s", AUTHOR);

  m_initPeripherals();

  ecs = ecs_new(128, nullptr);
  m_registerComponents();
  m_registerSystems();

  m_initEarlyData();
}
Engine::GameManager::~GameManager() {
  ecs_free(ecs);
  m_freePeripherals();
}
void Engine::GameManager::m_registerComponents() {
  log_info("Registering Components...");
  using namespace Components;
  m_initComponents(ecs);
}
void Engine::GameManager::m_registerSystems() {
  log_info("Registering Systems...");
  using namespace Systems;

  m_initSystems(ecs);
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
void Engine::GameManager::m_initEarlyData() {
  log_info("Initializing Early Data...");
  using namespace Components;

  m_makeMainMenu();
}
ecs_id_t Engine::GameManager::m_makeMainMenu() {
  ecs_id_t id = ecs_create(ecs);

  auto transform = AddComponent<Components::Transform>(ecs,id, TRANSFORM_COMP, nullptr);

  for (auto p: transform->Position)
    p = 0;
  for (auto p : transform->Rotation)
    p = 0;
  for (auto p: transform->Scale)
    p = 1;

  auto render = AddComponent<Components::Render>(ecs, id, TRANSFORM_COMP, nullptr);
  render->Data = m_mainMenu;
  render->Visible = true;
  render->x = 84;
  render->y = 48;

  return id;
}
