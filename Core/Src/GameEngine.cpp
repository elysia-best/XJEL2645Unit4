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
void Engine::BaseBehaviour::OnDestroyed() {}

Engine::BaseBehaviour::BaseBehaviour() : IsDisabled{false} {
  Start();
}

Engine::BaseBehaviour::~BaseBehaviour() {
  OnDestroyed();
}

void Engine::BaseBehaviour::AttachComponent(const BaseBehaviour* &ptr, bool delete_on_destroy) {
	AttachedComponent_t tmp = {ptr, delete_on_destroy};
	Elements_ptr_list.push_back(tmp);
}