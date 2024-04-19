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


volatile ecs_id_t TRANSFORM_SYSTEM;
volatile ecs_id_t RENDER_SYSTEM;


//namespace Systems {
//ecs_ret_t TransformSystem(ecs_t *ecs,
//                          ecs_id_t *entities,
//                          int entity_count,
//                          ecs_dt_t dt,
//                          void *udata) {
//  // log_info("TransformSystem");
//  return 0;
//}
//
//ecs_ret_t RenderSystem(ecs_t *ecs,
//                       ecs_id_t *entities,
//                       int entity_count,
//                       ecs_dt_t dt,
//                       void *udata) {
//
//    for (int i = 0; i < entity_count; i++) {
//      auto id = entities[i];
//      auto pos_comp = (Components::Transform *)ecs_get(GetGameManager(udata)->ecs, id, TRANSFORM_COMP);
//      auto render_comp = (Components::Render *)ecs_get(GetGameManager(udata)->ecs, id, RENDER_COMP);
//
//      if (render_comp->Visible) {
//        gameManager_ptr->lcd->drawSprite(pos_comp->Position[0], pos_comp->Position[1], render_comp->y, render_comp->x, render_comp->Data);
//      }
//    }
//  return 0;
//}
//void m_initSystems(ecs_s *ecs) {
//  TRANSFORM_SYSTEM = ecs_register_system(ecs, TransformSystem, nullptr, nullptr, gameManager_ptr);
//  RENDER_SYSTEM = ecs_register_system(ecs, RenderSystem, nullptr, nullptr, gameManager_ptr);
//
//  ecs_require_component(ecs, TRANSFORM_SYSTEM, TRANSFORM_COMP);
//
//  ecs_require_component(ecs, RENDER_SYSTEM, TRANSFORM_COMP);
//  ecs_require_component(ecs, RENDER_SYSTEM, RENDER_COMP);
//}
//}