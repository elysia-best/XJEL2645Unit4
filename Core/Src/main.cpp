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
#include "N5110.h"

N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);

Matrixf<3, 1> camera{14.77747, 29.36194, 27.99346}, X0{1, 0, 0}, Y0{0, 1, 0},
    Z0{0, 0, 1};
float Rotate_x, Rotate_y;

/**
 * @Name    main
 * @brief  Entry point for the app
 **/
int main() {
  // Init
  lcd.init(LPH7366_1);
  lcd.setContrast(0.5);

	float canvasWidth = 2, canvasHeight = 2;
	int imageWidth = 84, imageHeight = 48;

  while (true) {
    static Matrixf<3, 1> X=X0,Y=Y0,Z=Z0,FORWARD,RIGHT;
  }
}
