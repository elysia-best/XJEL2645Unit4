/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Systems.h
 * Author: Yuanzhen Gan
 * Date: 2024-04-09
 * Modify Record:
 */
#ifndef _SYSTEMS_H_
#define _SYSTEMS_H_

#include "pico_ecs.h"

namespace Systems {
extern ecs_id_t TRANSFORM_SYSTEM;
extern ecs_id_t RENDER_SYSTEM;
}

namespace Systems {
ecs_ret_t TransformSystem(ecs_t *ecs,
                       ecs_id_t *entities,
                       int entity_count,
                       ecs_dt_t dt,
                       void *udata);

ecs_ret_t RenderSystem(ecs_t *ecs,
                       ecs_id_t *entities,
                       int entity_count,
                       ecs_dt_t dt,
                       void *udata);
}

#endif