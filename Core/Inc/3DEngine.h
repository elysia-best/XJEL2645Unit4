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
                             float &canvasWidth,
                             float &canvasHeight, int &imageWidth,
                             int &imageHeight);

#endif
