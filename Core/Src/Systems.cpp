/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Systems.cpp
 * Author: Yuanzhen Gan
 * Date: 2024-04-09
 * Modify Record:
 */
#include "Systems.h"
#include "Components.h"
#include "GameEngine.h"
#include "log.h"
#include "GlobalDefines.h"
#include "ECS.h"
#include "tuple"

void Systems::TransformSystem::tick(ECS::World *world, float deltaTime) {
  world->each<Components::Transform>(
      [&](ECS::Entity *ent,
          ECS::ComponentHandle<Components::Transform> trans) -> void {
        trans->Position;
      }
  );
}
void Systems::RenderSystem::tick(ECS::World *world, float deltaTime) {
  world->each<Components::Transform, Components::Render>(
      [&](ECS::Entity *ent,
          ECS::ComponentHandle<Components::Transform> trans, ECS::ComponentHandle<Components::Render> render) -> void {
        using render_t = Components::Render::Type_e;
        if (render->Visible) {
          switch (render->Type) {
            case render_t::Spirit :
              Engine::GameManager::getInstance()->lcd->drawSprite(std::get<0>(trans->Position),std::get<1>(trans->Position), render->y, render->x, render->Data.spirit_Data);
              break;
          }

        }
      }
  );
}
