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
              Engine::GameManager::getInstance()->lcd->drawSprite(std::get<0>(trans->Position),std::get<1>(trans->Position), render->y, render->x, render->Data.spirit_Data, true);
              break;
          }

        }
      }
  );
}
void Systems::UIControlSystem::tick(ECS::World *world, float deltaTime) {
  world->each<Components::UIRender>(
      [&](ECS::Entity *ent, ECS::ComponentHandle<Components::UIRender> render) -> void {
          for (auto &p :render->m_comps) {
            if (p.id == render->selected) {
              Engine::GameManager::getInstance()->lcd->drawSprite(std::get<0>(p.trans.Position),std::get<1>(p.trans.Position), p.y, p.x, p.spirit_Data);
            }
          }

        }

  );
}
void Systems::UIControlSystem::receive(ECS::World *world, const Events::JoystickUpdateEvent &event) {
  int8_t control_result = 0;
  if (event.direction == Direction::E || event.direction == Direction::S) {
    control_result = 1;
  } else if (event.direction == Direction::W || event.direction == Direction::N) {
    control_result = -1;
  }

  if(control_result == 0)
    return;

  world->each<Components::UIRender>(
      [=](ECS::Entity *ent, ECS::ComponentHandle<Components::UIRender> render) -> void {
        if (control_result == 1) {
          render->selected = ((render->selected + 1) < (render->m_comps.size())) ? (render->selected + 1) : 0;
        } else if (control_result == 0) {
          render->selected = ((render->selected - 1) > 0) ? (render->selected - 1) : (render->m_comps.size()-1);
        }
      }
  );
}
