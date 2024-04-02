/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: GameEngine.cpp   
 * Author: Yuanzhen Gan
 * Date: 2024-04-01
 * Modify Record:
 */

#include "GameEngine.h"

void Engine::BaseBehaviour::Start() {}
void Engine::BaseBehaviour::Update() {}
void Engine::BaseBehaviour::Finalized() {}

Engine::BaseBehaviour::BaseBehaviour() {
  Start();
}

Engine::BaseBehaviour::~BaseBehaviour() {
  Finalized();
}