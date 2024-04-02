/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: main.cpp
 * Author: Yuanzhen Gan
 * Date: 2024-03-31
 * Modify Record:
 */

#include "main.h"

#include <mbed.h>

#include "3DEngine.h"
#include "Joystick.h"
#include "N5110.h"

//                  y     x
Joystick joystick(PC_1, PC_0);  // attach and create joystick object

N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);

Matrixf<3, 1> verts[146] = {
    {0, 39.034, 0},          {0.76212, 36.843, 0},   {3, 36.604, 0},
    {1, 35.604, 0},          {2.0162, 33.382, 0},    {0, 34.541, 0},
    {-2.0162, 33.382, 0},    {-1, 35.604, 0},        {-3, 36.604, 0},
    {-0.76212, 36.843, 0},   {-0.040181, 34.31, 0},  {3.2778, 30.464, 0},
    {-0.040181, 30.464, 0},  {-0.028749, 30.464, 0}, {3.2778, 30.464, 0},
    {1.2722, 29.197, 0},     {1.2722, 29.197, 0},    {-0.028703, 29.197, 0},
    {1.2722, 29.197, 0},     {5.2778, 25.398, 0},    {-0.02865, 25.398, 0},
    {1.2722, 29.197, 0},     {5.2778, 25.398, 0},    {3.3322, 24.099, 0},
    {-0.028683, 24.099, 0},  {7.1957, 20.299, 0},    {-0.02861, 20.299, 0},
    {5.2778, 19.065, 0},     {-0.028663, 18.984, 0}, {9.2778, 15.265, 0},
    {-0.028571, 15.185, 0},  {9.2778, 15.265, 0},    {7.3772, 13.999, 0},
    {-0.028625, 13.901, 0},  {9.2778, 15.265, 0},    {12.278, 8.9323, 0},
    {-0.028771, 8.9742, 0},  {12.278, 8.9323, 0},    {10.278, 7.6657, 0},
    {-0.028592, 7.6552, 0},  {15.278, 2.5994, 0},    {-0.028775, 2.6077, 0},
    {15.278, 2.5994, 0},     {13.278, 1.3329, 0},    {-0.028727, 1.2617, 0},
    {18.278, -3.7334, 0},    {18.278, -3.7334, 0},   {2.2722, -1.2003, 0},
    {-0.028727, -1.3098, 0}, {4.2722, -5, 0},        {4.2722, -5, 0},
    {-0.028727, -5, 0},      {-3.3582, 30.464, 0},   {-3.3582, 30.464, 0},
    {-1.3526, 29.197, 0},    {-1.3526, 29.197, 0},   {-1.3526, 29.197, 0},
    {-5.3582, 25.398, 0},    {-1.3526, 29.197, 0},   {-5.3582, 25.398, 0},
    {-3.4126, 24.099, 0},    {-7.276, 20.299, 0},    {-5.3582, 19.065, 0},
    {-9.3582, 15.265, 0},    {-9.3582, 15.265, 0},   {-7.4575, 13.999, 0},
    {-9.3582, 15.265, 0},    {-12.358, 8.9323, 0},   {-12.358, 8.9323, 0},
    {-10.358, 7.6657, 0},    {-15.358, 2.5994, 0},   {-15.358, 2.5994, 0},
    {-13.358, 1.3329, 0},    {-18.358, -3.7334, 0},  {-18.358, -3.7334, 0},
    {-2.3526, -1.2003, 0},   {-4.3526, -5, 0},       {-4.3526, -5, 0},
    {0, 34.31, 0.040181},    {0, 30.464, -3.2778},   {0, 30.464, 0.040181},
    {0, 30.464, 0.028749},   {0, 30.464, -3.2778},   {0, 29.197, -1.2722},
    {0, 29.197, -1.2722},    {0, 29.197, 0.028703},  {0, 29.197, -1.2722},
    {0, 25.398, -5.2778},    {0, 25.398, 0.02865},   {0, 29.197, -1.2722},
    {0, 25.398, -5.2778},    {0, 24.099, -3.3322},   {0, 24.099, 0.028683},
    {0, 20.299, -7.1957},    {0, 20.299, 0.02861},   {0, 19.065, -5.2778},
    {0, 18.984, 0.028663},   {0, 15.265, -9.2778},   {0, 15.185, 0.028571},
    {0, 15.265, -9.2778},    {0, 13.999, -7.3772},   {0, 13.901, 0.028625},
    {0, 15.265, -9.2778},    {0, 8.9323, -12.278},   {0, 8.9742, 0.028771},
    {0, 8.9323, -12.278},    {0, 7.6657, -10.278},   {0, 7.6552, 0.028592},
    {0, 2.5994, -15.278},    {0, 2.6077, 0.028775},  {0, 2.5994, -15.278},
    {0, 1.3329, -13.278},    {0, 1.2617, 0.028727},  {0, -3.7334, -18.278},
    {0, -3.7334, -18.278},   {0, -1.2003, -2.2722},  {0, -1.3098, 0.028727},
    {0, -5, -4.2722},        {0, -5, -4.2722},       {0, -5, 0.028727},
    {0, 30.464, 3.3582},     {0, 30.464, 3.3582},    {0, 29.197, 1.3526},
    {0, 29.197, 1.3526},     {0, 29.197, 1.3526},    {0, 25.398, 5.3582},
    {0, 29.197, 1.3526},     {0, 25.398, 5.3582},    {0, 24.099, 3.4126},
    {0, 20.299, 7.276},      {0, 19.065, 5.3582},    {0, 15.265, 9.3582},
    {0, 15.265, 9.3582},     {0, 13.999, 7.4575},    {0, 15.265, 9.3582},
    {0, 8.9323, 12.358},     {0, 8.9323, 12.358},    {0, 7.6657, 10.358},
    {0, 2.5994, 15.358},     {0, 2.5994, 15.358},    {0, 1.3329, 13.358},
    {0, -3.7334, 18.358},    {0, -3.7334, 18.358},   {0, -1.2003, 2.3526},
    {0, -5, 4.3526},         {0, -5, 4.3526}};
const int numTris = 128;
// tris[n*3],tris[n*3+1],tris[n*3+2]分别为第n个三角形的三个顶点在verts[]的编号
int tris[numTris * 3] = {
    8,   7,   9,   6,   5,   7,   4,   3,   5,   2,   1,   3,   0,   9,   1,
    5,   3,   7,   7,   3,   9,   9,   3,   1,   10,  12,  11,  13,  15,  14,
    15,  13,  16,  13,  17,  16,  18,  20,  19,  17,  20,  21,  20,  23,  22,
    20,  24,  23,  23,  26,  25,  24,  26,  23,  26,  27,  25,  26,  28,  27,
    27,  30,  29,  28,  30,  27,  30,  32,  31,  30,  33,  32,  27,  30,  34,
    32,  36,  35,  33,  36,  32,  36,  38,  37,  36,  39,  38,  38,  41,  40,
    39,  41,  38,  41,  43,  42,  41,  44,  43,  44,  45,  43,  44,  47,  46,
    44,  48,  47,  48,  49,  47,  48,  51,  50,  10,  52,  12,  13,  53,  54,
    55,  17,  54,  13,  54,  17,  56,  57,  20,  17,  58,  20,  20,  59,  60,
    20,  60,  24,  60,  61,  26,  24,  60,  26,  26,  61,  62,  26,  62,  28,
    62,  63,  30,  28,  62,  30,  30,  64,  65,  30,  65,  33,  62,  66,  30,
    65,  67,  36,  33,  65,  36,  36,  68,  69,  36,  69,  39,  69,  70,  41,
    39,  69,  41,  41,  71,  72,  41,  72,  44,  44,  72,  73,  44,  74,  75,
    44,  75,  48,  48,  75,  76,  48,  77,  51,  78,  80,  79,  81,  83,  82,
    83,  81,  84,  81,  85,  84,  86,  88,  87,  85,  88,  89,  88,  91,  90,
    88,  92,  91,  91,  94,  93,  92,  94,  91,  94,  95,  93,  94,  96,  95,
    95,  98,  97,  96,  98,  95,  98,  100, 99,  98,  101, 100, 95,  98,  102,
    100, 104, 103, 101, 104, 100, 104, 106, 105, 104, 107, 106, 106, 109, 108,
    107, 109, 106, 109, 111, 110, 109, 112, 111, 112, 113, 111, 112, 115, 114,
    112, 116, 115, 116, 117, 115, 116, 119, 118, 78,  120, 80,  81,  121, 122,
    123, 85,  122, 81,  122, 85,  124, 125, 88,  85,  126, 88,  88,  127, 128,
    88,  128, 92,  128, 129, 94,  92,  128, 94,  94,  129, 130, 94,  130, 96,
    130, 131, 98,  96,  130, 98,  98,  132, 133, 98,  133, 101, 130, 134, 98,
    133, 135, 104, 101, 133, 104, 104, 136, 137, 104, 137, 107, 137, 138, 109,
    107, 137, 109, 109, 139, 140, 109, 140, 112, 112, 140, 141, 112, 142, 143,
    112, 143, 116, 116, 143, 144, 116, 145, 119};

const int canvasWidth = 2, canvasHeight = 2;
const int imageWidth = 84, imageHeight = 48;

void read_joystick();

/**
 * @Name    main
 * @brief  Entry point for the app
 **/
int main() {
  // Init
  lcd.init(LPH7366_1);
  lcd.setContrast(0.5);
  joystick.init();

//                      x         y        z
  Matrixf<3, 1> camera{0.77747, 10.36194, 27.99346}, X0{1, 0, 0}, Y0{0, 1, 0},
    Z0{0, 0, 1};

// Default to zero in global definition, haha.
  float Rotate_x, Rotate_y;
  while (true) {
    Matrixf<3, 1> X = X0, Y = Y0, Z = Z0, FORWARD, RIGHT;

    rotate_point3d_y(X, Rotate_y, X);
    rotate_point3d_y(Y, Rotate_y, Y);
    rotate_point3d_y(Z, Rotate_y, Z);

    rotate_point3d_x(X, Rotate_x, X);
    rotate_point3d_x(Y, Rotate_x, Y);
    rotate_point3d_x(Z, Rotate_x, Z);

    Matrixf<4, 4> cameraToWorld = {
        X[0][0],      Y[0][0],      Z[0][0],      0,
        X[1][0],      Y[1][0],      Z[1][0],      0,
        X[2][0],      Y[2][0],      Z[2][0],      0,
        camera[0][0], camera[1][0], camera[2][0], 1};
    Matrixf<4, 4> worldToCamera = matrixf::inv(cameraToWorld);

    lcd.clear();

    for (int i = 0; i < numTris; ++i) {
      auto &v0World = verts[tris[i * 3]];
      auto &v1World = verts[tris[i * 3 + 1]];
      auto &v2World = verts[tris[i * 3 + 2]];
      Matrixf<2, 1> v0Raster, v1Raster, v2Raster;
      bool show0 =
          computePixelCoordinates(v0World, v0Raster, worldToCamera, canvasWidth,
                                  canvasHeight, imageWidth, imageHeight);
      bool show1 =
          computePixelCoordinates(v1World, v1Raster, worldToCamera, canvasWidth,
                                  canvasHeight, imageWidth, imageHeight);
      bool show2 =
          computePixelCoordinates(v2World, v2Raster, worldToCamera, canvasWidth,
                                  canvasHeight, imageWidth, imageHeight);

      if (show0 && show1)
        lcd.drawLine(v0Raster[0][0] < 0 ? 0 : v0Raster[0][0],
                     v0Raster[1][0] < 0 ? 0 : v0Raster[1][0],
                     v1Raster[0][0] < 0 ? 0 : v1Raster[0][0],
                     v1Raster[1][0] < 0 ? 0 : v1Raster[1][0], 1);
      if (show0 && show2)
        lcd.drawLine(v0Raster[0][0] < 0 ? 0 : v0Raster[0][0],
                     v0Raster[1][0] < 0 ? 0 : v0Raster[1][0],
                     v2Raster[0][0] < 0 ? 0 : v2Raster[0][0],
                     v2Raster[1][0] < 0 ? 0 : v2Raster[1][0], 1);
      if (show2 && show1)
        lcd.drawLine(v2Raster[0][0] < 0 ? 0 : v2Raster[0][0],
                     v2Raster[1][0] < 0 ? 0 : v2Raster[1][0],
                     v1Raster[0][0] < 0 ? 0 : v1Raster[0][0],
                     v1Raster[1][0] < 0 ? 0 : v1Raster[1][0], 1);

      //      if (show0 && show1)
      //        printf("%d %d %d %d\n", (int)v0Raster[0][0],
      //        (int)v0Raster[1][0], (int)v1Raster[0][0],
      //               (int)v1Raster[1][0]);
      //      if (show0 && show2)
      //        printf("%d %d %d %d\n", (int)v0Raster[0][0],
      //        (int)v0Raster[1][0], (int)v2Raster[0][0],
      //               (int)v2Raster[1][0]);
      //      if (show2 && show1)
      //        printf("%d %d %d %d\n", (int)v2Raster[0][0],
      //        (int)v2Raster[1][0], (int)v1Raster[0][0],
      //               (int)v1Raster[1][0]);
    }
    lcd.refresh();
    // thread_sleep_for(10);
//    static Matrixf<3, 1> S1{worldToCamera[0][2], 0.0, worldToCamera[2][2]};
//    static Matrixf<3, 1> D1{worldToCamera[0][0], 0.0, worldToCamera[2][0]};
//    S1.normalize();
//    D1.normalize();
    camera = camera + 3*Y0;
  }
}

void read_joystick() {}
