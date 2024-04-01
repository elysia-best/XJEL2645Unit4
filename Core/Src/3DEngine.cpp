/*
 * (C) Copyright 2024 Yuanzhen Gan and wangyuhan2010. All Rights Reserved.
 * Description: 3DEngine.cpp
 * Author: Yuanzhen Gan
 * Date: 2024-03-31
 * Original Author: wangyuhan2010,
 * https://blog.csdn.net/wangyuhan2010/article/details/130477415 Adapted and
 * modified by: Yuanzhen Gan Modify Record:
 */

#include "3DEngine.h"

bool computePixelCoordinates(Matrixf<3, 1> &pWorld, Matrixf<2, 1> &pRaster,
                             Matrixf<4, 4> &worldToCamera, const int &canvasWidth,
                             const int &canvasHeight, const int &imageWidth,
                             const int &imageHeight) {
  Matrixf<3, 1> pCamera = pCamera.multVecMatrix(pWorld, worldToCamera);
  if (pCamera[2][0] > 0) return 0;
  Matrixf<2, 1> pScreen;
  pScreen[0][0] = pCamera[0][0] / -pCamera[2][0];  // x
  pScreen[1][0] = pCamera[1][0] / -pCamera[2][0];  // y
  Matrixf<2, 1> pNDC;
  pNDC[0][0] = (pScreen[0][0] + (float)canvasWidth * 0.5) / (float)canvasWidth;
  pNDC[1][0] = (pScreen[1][0] + (float)canvasHeight * 0.5) / (float)canvasHeight;
  pRaster[0][0] = (int)(pNDC[0][0] * imageWidth);
  pRaster[1][0] = (int)((1 - pNDC[1][0]) * imageHeight);
  return 1;
}

void rotate_point3d_x(Matrixf<3, 1> &vec, float angle, Matrixf<3, 1> &dst) {
  float cos_val, sin_val;
  arm_sin_cos_f32(angle, &sin_val, &cos_val);
  Matrixf<3,3> roationMat = {
		1, 0,       0,
    0, cos_val, -sin_val,
    0, sin_val, cos_val
	};
  dst = roationMat * vec;
}

void rotate_point3d_y(Matrixf<3, 1> &vec, float angle, Matrixf<3, 1> &dst) {
  float cos_val, sin_val;
  arm_sin_cos_f32(angle, &sin_val, &cos_val);
  Matrixf<3,3> roationMat = {
		cos_val,  0, sin_val,
    0,        1, 0,
    -sin_val, 0, cos_val
	};
  dst = roationMat * vec;
}
