/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: main.cpp
 * Author: Yuanzhen Gan
 * Date: 2024-03-31
 * Modify Record:
 */

#define PICO_ECS_IMPLEMENTATION

#include "main.h"

#include <mbed.h>

#include "3DEngine.h"
#include "Joystick.h"
#include "N5110.h"

#include "GlobalDefines.h"


#include "GameEngine.h"
#include "Spirits.h"
#include "Components.h"

ECS_TYPE_IMPLEMENTATION

int main() {
  Engine::GameManager::getInstance()->Init();

  static float dt = 0.0f;

  while (true) {
    static auto startTime = Kernel::Clock::now();

    Engine::GameManager::getInstance()->ecs->tick(dt);

    Engine::GameManager::getInstance()->lcd->refresh();

    auto stopTime = Kernel::Clock::now();

    dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
  }
}

