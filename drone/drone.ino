/*
 * Startcode
 * 2022-07-10 GEE, updated 2023-03-22 GEE
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

RMTT_TOF tt_sensor;

RMTT_RGB tt_rgb;

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
  Wire.begin(27, 26);
  Wire.setClock(400000);

  // matrix display initialiseren
  tt_matrix.Init(127);                                                       
  tt_matrix.SetLEDStatus(RMTT_MATRIX_CS, RMTT_MATRIX_SW, RMTT_MATRIX_LED_ON); 

  // afstandsensor initialiseren
  tt_sensor.SetTimeout(500); 
  if (!tt_sensor.Init()) {
     Serial.println("Failed to detect and initialize sensor!");
     while (1) {}
  }
  
  // tello initialiseren
  Serial.begin(115200); // verbinding tussen RMTT en computer starten
  Serial.println("Serial init done");
  Serial1.begin(1000000, SERIAL_8N1, 23, 18); // verbinding tussen RMTT en drone starten
  tello.startUntilControl(); // wacht tot Tello gereed is om te starten en knopje op RMTT is ingedrukt
  Serial.println("Tello init done");
  // tello laten vliegen. 
  // Er zijn allerlei commando's die de drone naar een bepaalde plek sturen, zie de SDK 3.0 documentatie uit de repo. 
  // Die plek is wel onnauwkeurig.
  // Als de drone een mission pad herkent, dan kun je de drone naar een plek sturen ten opzichte van het mission pad 
  // (in plaats van ten opzichte van opstijgen), dat helpt met de nauwkeurigheid
  
  tt_matrix.SetAllPWM((uint8_t *)matrix_b1); // zet 1 op matrix
  tello.sendTelloCtrlMsg("takeoff"); // opstijgen, pas als de drone opgestegen is gaat het programma naar de volgende regel
  
  tt_matrix.SetAllPWM((uint8_t *)matrix_b2); // zet 2 op matrix
  // tello.sendTelloCtrlMsg("left 30"); zorgt dat de drone het commando uitvoert en daarna pas met de volgende opdracht verder gaat
  
  // tello.Left(30); delay(10000) werkt ook, maar het programma wacht dan altijd 10 sec, tenzij je zelf de response van tello uitleest
 
  // Serial1.printf("[TELLO] left 30"); delay (10000) werkt ook,  maar het programma wacht dan altijd 10 sec, tenzij je zelf de response van tello uitleest
  
  // void RMTT_Protocol::SendCMD(char *cmd) => voor verzenden commando zonder wachten, 
  // helaas is dit een private method waardoor we die niet kunnen aanroepen
  // we doen het daarom kort door de bocht
  // Serial1.printf("[TELLO] %s", "left 30");
  // daarna herhaaldelijk het antwoord van de drone bekijken tot de drone klaar is
  
  // DUS OP DEZE MANIER MOET HET, ANDERS GEEN VOLDOENDE:
  Serial1.printf("[TELLO] %s", "cw 300");
  String response = "timeout";
  uint16_t range = 8192;
  while(response == "timeout" && range > 500) {
    range = tt_sensor.ReadRangeSingleMillimeters(); // afstand in mm, sensor leest tot ongeveer 1000 mm, 8192 betekent buiten range
    if (tt_sensor.TimeoutOccurred()) {Serial.print("TIMEOUT reading TOF");}
    response = tello.getTelloResponseString(100); // response is "timeout" als na 100 ms nog geen bericht is ontvangen (de drone is dan nog bezig met de laatste opdracht)
    Serial.println(response);
  }
  // string kan ok of error zijn, we gaan uit van ok
  Serial1.printf("[TELLO] %s", "stop"); // vorige commando waarmee de drone bezig is wordt afgebroken
  
  tt_matrix.SetAllPWM((uint8_t *)matrix_b3); // zet 3 op matrix
  // tello.sendTelloCtrlMsg("flip l"); // flip is draai over de kop, niet nuttig, wel leuk
  
  tello.sendTelloCtrlMsg("right 30"); // programma gaat door, maar drone maakt eerst commando af
  tello.sendTelloCtrlMsg("land"); // commando wordt door de drone gebufferd en gestart zodra vorige commando klaar is
}

void loop() {
  // hier toestandsdiagrammencode toevoegen, zie de andere onderwerpen van deze opdracht
  // LETOP: ALS JE CODE NIET IS OPGEBOUWD MET TOESTANDSDIAGRAMMEN EN ZOALS IN DE LES BEHANDELD, DAN GEEN VOLDOENDE
  Serial.println("loop gestart");
  delay(1000);
  
}
