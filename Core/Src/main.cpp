/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: main.cpp
 * Author: Yuanzhen Gan
 * Date: 2024-03-31
 * Modify Record:
 */

#include "main.h"

#include <mbed.h>

#include "arm_math.h"

/**
 * @Name    main
 * @brief  Entry point for the app
 **/
int main() {
  int16_t i;
  float32_t pSinVal;
  float32_t pCosVal;
  for (i = -180; i < 180; i++) {
    arm_sin_cos_f32(i, &pSinVal, &pCosVal);
    printf("%d,%d\n", (int)pSinVal, (int)pCosVal);
  }
  while (true) {
  }
}
