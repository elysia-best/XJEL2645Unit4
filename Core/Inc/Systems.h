/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Systems.h
 * Author: Yuanzhen Gan
 * Date: 2024-04-09
 * Modify Record:
 */
#ifndef _SYSTEMS_H_
#define _SYSTEMS_H_

#include "ECS.h"

namespace Systems {


//ecs_ret_t TransformSystem(ecs_t *ecs,
//                       ecs_id_t *entities,
//                       int entity_count,
//                       ecs_dt_t dt,
//                       void *udata);
//
//ecs_ret_t RenderSystem(ecs_t *ecs,
//                       ecs_id_t *entities,
//                       int entity_count,
//                       ecs_dt_t dt,
//                       void *udata);

 struct TransformSystem : public ECS::EntitySystem {
   ~TransformSystem() override = default;

   void tick(ECS::World* world, float deltaTime) override;
 };

 struct RenderSystem : public ECS::EntitySystem {
   ~RenderSystem() override = default;

   void tick(ECS::World* world, float deltaTime) override;
 };
}

#endif