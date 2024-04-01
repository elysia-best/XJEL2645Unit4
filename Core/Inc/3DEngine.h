/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: 3DEngine.h
 * Author: Yuanzhen Gan
 * Date: 2024-03-31
 * Modify Record:
 */

#ifndef __3_D_ENGINE_H_
#define __3_D_ENGINE_H_

#include "matrix.h"

/**
 * @Name  computePixelCoordinates
 * @brief 计算在世界空间中定义的点的二维像素坐标。
 * @author wangyuhan2010, Yuanzhen Gan
 **/
bool computePixelCoordinates(Matrixf<3, 1>& pWorld, Matrixf<2, 1> &pRaster,
                             Matrixf<4, 4> &worldToCamera,
                             const int &canvasWidth,
                             const int &canvasHeight, const int &imageWidth,
                             const int &imageHeight);

/**
  * @Name    rotate_point3d_x
  * @brief   Rotate a vector around x axis.
  * @param   vec: Input vector
  **			   angle:  Rotation angle
  **         dst: Output result
  * @retval  void
  * @author  Yuanzhen Gan
  * @Data    2024-04-01
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
  **/
void rotate_point3d_x(Matrixf<3,1>& vec, float angle, Matrixf<3,1>& dst);

/**
  * @Name    rotate_point3d_y
  * @brief   Rotate a vector around y axis
  * @param   vec: Input vector
  **			   angle: Rotation angle
  **			   dst: Output result
  * @retval  void
  * @author  Yuanzhen Gan
  * @Data    2024-04-01
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
  **/
void rotate_point3d_y(Matrixf<3,1>& vec, float angle, Matrixf<3,1>& dst);
#endif
