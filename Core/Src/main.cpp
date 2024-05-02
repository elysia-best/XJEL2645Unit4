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

volatile uint16_t dt = 0.0f;

int main() {
  Engine::GameManager::getInstance()->Init();

  while(1) {
    static auto startTime = HAL_GetTick();

    Engine::GameManager::getInstance()->m_checkPeripherals();
    Engine::GameManager::getInstance()->ecs->tick(dt);
    Engine::GameManager::getInstance()->lcd->refresh();

    static auto stopTime = HAL_GetTick();

    dt = stopTime - startTime; // ms
  }

}

