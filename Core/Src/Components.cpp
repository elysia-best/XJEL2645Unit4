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
ECS_DEFINE_TYPE(Transform);

ECS_DEFINE_TYPE(Render);

ECS_DEFINE_TYPE(UIRender);

ECS_DEFINE_TYPE(Note);
};