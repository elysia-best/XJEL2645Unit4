/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Entities.cpp
 * Author: Yuanzhen Gan
 * Date: 24-4-13
 * Modify Record:
 */
#include "Entities.h"
#include "Components.h"

namespace Entities {
class UI {
  Components::Render render;
  Components::Transform transform;
};
}