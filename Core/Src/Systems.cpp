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

namespace Systems {
ecs_id_t TRANSFORM_SYSTEM;
ecs_id_t RENDER_SYSTEM;
}

namespace Systems {
ecs_ret_t TransformSystem(ecs_t *ecs,
                          ecs_id_t *entities,
                          int entity_count,
                          ecs_dt_t dt,
                          void *udata) {
  log_info("TransformSystem");
  return 0;
}

ecs_ret_t RenderSystem(ecs_t *ecs,
                       ecs_id_t *entities,
                       int entity_count,
                       ecs_dt_t dt,
                       void *udata) {

//    for (int i = 0; i < entity_count; i++) {
//      auto id = entities[i];
//      auto pos_comp = (Components::Transform *)ecs_get(GetGameManager(udata)->ecs, id, Components::TRANSFORM_COMP);
//      auto render_comp = (Components::Render *)ecs_get(GetGameManager(udata)->ecs, id, Components::RENDER_COMP);
//
//      if (render_comp->Visible) {
//      }
//    }
  log_info("RenderSystem");
  return 0;
}
}