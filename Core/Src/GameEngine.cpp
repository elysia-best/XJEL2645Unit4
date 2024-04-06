/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: GameEngine.cpp   
 * Author: Yuanzhen Gan
 * Date: 2024-04-01
 * Modify Record:
 */

#include "GameEngine.h"

Engine::GameManager::GameManager() {
  ecs = ecs_new(128, nullptr);
}
Engine::GameManager::~GameManager() {
  ecs_free(ecs);
}
