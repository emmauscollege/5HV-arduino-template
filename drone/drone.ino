/*
 * Startcode
 * 2022-07-10 GEE
 * Status: work in progress
 * 
 * Gebaseerd op de voorbeelden die in de library van de drone worden meegeleverd
 * Library:
 * Copyright (C) 2020 DJI.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     robomaster   first version
 */

#include <RMTT_Libs.h>
#include <Wire.h>

RMTT_Matrix tt_matrix;

RMTT_Protocol tello;

// symbool 1 maken voor weergave op matrix
uint8_t matrix_b1[] = {
      0,   0,   0,   0,   0,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 255,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};
// symbool 2 maken voor weergave op matrix
uint8_t matrix_b2[] = {
      0,   0,   0,   0,   0,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 255,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 255,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 255,   0, 255,   0, 255,   0, 255,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};
// symbool 3 maken voor weergave op matrix
uint8_t matrix_b3[] = {
      0,   0,   0,   0,   0,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 255,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 255,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   0, 255,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};



void setup() {
  // matrix initialiseren
  Wire.begin(27, 26);
  Wire.setClock(400000);
  tt_matrix.Init(127);                                                       
  tt_matrix.SetLEDStatus(RMTT_MATRIX_CS, RMTT_MATRIX_SW, RMTT_MATRIX_LED_ON); 
  
  // tello initialiseren
  Serial.begin(115200); // verbinding tussen RMTT en computer starten
  Serial1.begin(1000000, SERIAL_8N1, 23, 18); // verbinding tussen RMTT en drone starten
  tello.startUntilControl(); // wacht tot Tello gereed is om te starten en knopje op RMTT is ingedrukt

  // tello laten vliegen. 
  // Er zijn allerlei commando's die de drone naar een bepaalde plek sturen, zie de SDK 3.0 documentatie uit de repo. 
  // Die plek is wel onnauwkeurig.
  // Als de drone een mission pad herkent, dan kun je de drone naar een plek sturen ten opzichte van het mission pad 
  // (in plaats van ten opzichte van opstijgen), dat helpt met de nauwkeurigheid
  
  tt_matrix.SetAllPWM((uint8_t *)matrix_b1); // zet 1 op matrix
  tello.sendTelloCtrlMsg("takeoff"); // opstijgen, pas als de drone opgestegen is gaat het programma naar de volgende regel
  
  tt_matrix.SetAllPWM((uint8_t *)matrix_b2); // zet 2 op matrix
  tello.sendTelloCtrlMsg("left 30"); // wat niet werkt: tello.Left(30); => later uitzoeken
  
  tt_matrix.SetAllPWM((uint8_t *)matrix_b3); // zet 3 op matrix
  // tello.sendTelloCtrlMsg("flip l"); // flip is draai over de kop, niet nuttig, wel leuk
  
  tello.sendTelloCtrlMsg("right 30");
  
  tello.sendTelloCtrlMsg("land");
}

void loop() {
  
}
