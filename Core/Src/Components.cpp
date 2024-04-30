/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Components.cpp
 * Author: Yuanzhen Gan
 * Date: 2024-04-06
 * Modify Record:
 */
#include "Components.h"

using namespace ECS;

namespace Components {
Internal::TypeRegistry Transform::__ecs_type_reg;

Internal::TypeRegistry Render::__ecs_type_reg;

Internal::TypeRegistry UIRender::__ecs_type_reg;
};