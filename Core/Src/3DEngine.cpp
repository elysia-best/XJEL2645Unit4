/*
 * (C) Copyright 2024 Yuanzhen Gan and wangyuhan2010. All Rights Reserved.
 * Description: 3DEngine.cpp   
 * Author: Yuanzhen Gan
 * Date: 2024-03-31
 * Original Author: wangyuhan2010, https://blog.csdn.net/wangyuhan2010/article/details/130477415
 * Adapted and modified by: Yuanzhen Gan
 * Modify Record:
 */

#include "3DEngine.h"

bool computePixelCoordinates(Matrixf<3, 1>& pWorld, Matrixf<2, 1> &pRaster,
                             Matrixf<4, 4> &worldToCamera,
                             float &canvasWidth,
                             float &canvasHeight, int &imageWidth,
                             int &imageHeight) {
  Matrixf<3, 1> pCamera = pCamera.multVecMatrix(pWorld, worldToCamera);
  if (pCamera[2][0] > 0) return 0;
  Matrixf<2, 1> pScreen;
  pScreen[0][0] = pCamera[0][0] / -pCamera[2][0]; // x
  pScreen[1][0] = pCamera[1][0] / -pCamera[2][0]; // y
  Matrixf<2, 1> pNDC;
  pNDC[0][0] = (pScreen[0][0] + canvasWidth * 0.5) / canvasWidth;
  pNDC[1][0] = (pScreen[1][0] + canvasHeight * 0.5) / canvasHeight;
  pRaster[0][0] = (int)(pNDC[0][0] * imageWidth);
  pRaster[1][0] = (int)((1 - pNDC[1][0]) * imageHeight);
  return 1;
}

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
void rotate_point3d_x(Matrixf<3,1>& vec, float angle, Matrixf<3,1>& dst) {
	float cos_val, sin_val;
  arm_sin_cos_f32(angle, &sin_val, &cos_val);
  Matrixf<3,3> roationMat = {
		1, 0, 0,
    0, cos_val, -sin_val,
    0, sin_val, cos_val
	};
  dst = roationMat * vec;
}

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
void rotate_point3d_y(Matrixf<3,1>& vec, float angle, Matrixf<3,1>& dst) {
	float cos_val, sin_val;
  arm_sin_cos_f32(angle, &sin_val, &cos_val);
  Matrixf<3,3> roationMat = {
		cos_val, 0, sin_val,
    0, 1, 0,
    -sin_val, 0, cos_val
	};
  dst = roationMat * vec;
}
