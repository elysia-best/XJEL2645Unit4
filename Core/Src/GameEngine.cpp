/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: GameEngine.cpp   
 * Author: Yuanzhen Gan
 * Date: 2024-04-01
 * Modify Record:
 */

#include "GameEngine.h"
#include "Components.h"

Engine::GameManager::GameManager() {
  ecs = ecs_new(128, nullptr);
  m_registerComponents();
}
Engine::GameManager::~GameManager() {
  ecs_free(ecs);
}
void Engine::GameManager::m_registerComponents() {
  using namespace Components;
  TRANSFORM_COMP = ecs_register_component(ecs, sizeof(Transform), nullptr, nullptr);
  PLAYER_COMP = ecs_register_component(ecs, sizeof(Player), nullptr, nullptr);
  NOTE_COMP = ecs_register_component(ecs, sizeof(Note), nullptr, nullptr);
  RENDER_COMP = ecs_register_component(ecs, sizeof(Render), nullptr, nullptr);
}


