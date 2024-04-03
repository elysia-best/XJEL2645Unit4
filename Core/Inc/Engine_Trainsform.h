/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: Engine_Trainsform.h
 * Author: Yuanzhen Gan
 * Date: 2024-04-02
 * Modify Record:
 */

#ifndef __ENGINE__TRAINSFORM_H_
#define __ENGINE__TRAINSFORM_H_

#include "GameEngine.h"
#include "matrix.h"

namespace Engine {
class Transfrom : public BaseBehaviour {
 public:
  /**
   * @breif Position <x, y, z> of the object, in meter
   **/
  Matrixf<3, 1> position;

  /**
   * @breif roation <x, y, z> of the object, in degree
   **/
  Matrixf<3, 1> rotation;
};
}  // namespace Engine

#endif
