/*****************************************
   Drone Startcode
   met toestansdiagrammen
   Emmauscollege
   2022/10/03-2023/08/29 GEE
 *****************************************/

/*****************************************
   libraries die je gebruikt 

   Libraries van de fabrikant van de Tello drone
   https://github.com/RoboMaster/RMTT_Libs

 *****************************************/

#include <RMTT_Libs.h>
#include <Wire.h>


/*****************************************
   variabelen die je gebruikt maken
 *****************************************/
// gebruikte componenten op de drone
RMTT_Protocol tello;     // drone
RMTT_Matrix   tt_matrix; // display op de drone
RMTT_TOF      tt_sensor; // afstandssensor (TOF, dat staat voor Time Of Flight, deze zit boven het display)
RMTT_RGB      tt_rgb;    // led bovenop de drone

// variabelen om waarden van sensoren en actuatoren te onthouden
uint16_t afstand = 0;        // afstand gemeten door afstandssensor
String response = "timeout"; // bericht van drone
long int startTijd = millis();

// variabelen voor de toestanden
const int OPSTIJGEN = 1; // tel af tot spel start
const int DRAAIEN = 2; // speel het spel
const int LANDEN   = 3; // laat zien wie de winnaar is
int toestand = OPSTIJGEN;
unsigned long toestandStartTijd = 0;

// variabelen met tekens die je op het matrix display kunt zetten

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

/*****************************************
   functies die je gebruikt maken
 *****************************************/

/*****************************************
   setup() en loop()
 *****************************************/
 
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
  // (in plaats van ten opzichte van opstijgen), dat helpt met de nauwkeurigheid, 
  // in deze voorbeeldcode gebruiken we geen mission pads

  // init eerste toestand
  unsigned long toestandStartTijd = millis();
  Serial1.printf("[TELLO] %s", "takeoff"); // start met opstijgen, uitvoeren van code gaat door terwijl opstijgen bezig is.

  // Hier zijn een paar manieren die door je docent getest zijn om de drone te besturen
  // tello.sendTelloCtrlMsg("left 30"); zorgt dat de drone het commando uitvoert en daarna pas met de volgende opdracht verder gaat  
  // tello.Left(30); delay(10000) werkt ook, maar het programma wacht dan altijd 10 sec, tenzij je zelf de response van tello uitleest 
  // Serial1.printf("[TELLO] left 30"); delay (10000) werkt ook,  maar het programma wacht dan altijd 10 sec, tenzij je zelf de response van tello uitleest
  // void RMTT_Protocol::SendCMD(char *cmd) => voor verzenden commando zonder wachten, 
  // helaas is dit een private method waardoor we die niet kunnen aanroepen
  // we doen het daarom kort door de bocht
  // Serial1.printf("[TELLO] %s", "left 30");
  // daarna herhaaldelijk het antwoord van de drone bekijken tot de drone klaar is
}

void loop() {
  // hier toestandsdiagrammencode toevoegen, zie de andere onderwerpen van deze opdracht
  // LETOP: ALS JE CODE NIET IS OPGEBOUWD MET TOESTANDSDIAGRAMMEN EN ZOALS IN DE LES BEHANDELD, DAN GEEN VOLDOENDE
  Serial.println("loop gestart");
  delay(1000);

  // lees sensorwaarden
  response = "timeout";
  response = tello.getTelloResponseString(100); // response is "timeout" als na 100 ms nog geen bericht is ontvangen (de drone is dan nog bezig met de laatste opdracht)
  if (response != "timeout") {Serial.print("TT drone response:");Serial.print(response);}
  // response is timeout (niks beschikbaar), ok (commando uitgevoerd) of error... (er is iets mis gegaan)
  // zie SDK documentatie voor meer informatie over response

  afstand = tt_sensor.ReadRangeSingleMillimeters(); // afstand in mm, sensor leest tot ongeveer 1000 mm, 8192 betekent buiten range
  if (tt_sensor.TimeoutOccurred()) {Serial.println("ERROR: timeout reading TOF");}

  // bepaal toestand
  if (toestand == OPSTIJGEN) {
    if (response == "ok") { // drone klaar met opstijgen
      toestand = DRAAIEN;
      Serial.println("Nieuwe toestand: DRAAIEN");
    }
  }
  if (toestand == DRAAIEN) {
    if (afstand < 500) { // afstandssensor ziet iets op minder dan 50 cm (bijvoorbeeld een hand)
      Serial1.printf("[TELLO] %s", "stop"); // vorige commando (draaien) waarmee de drone bezig is wordt afgebroken
      toestand = LANDEN;
      Serial.println("Nieuwe toestand: LANDEN");
    }
  }
  if (toestand == LANDEN) {
    // drone blijft in deze toestand tot reset
  }

  // zet actuatoren
  if (toestand == OPSTIJGEN) {
    tt_matrix.SetAllPWM((uint8_t *)matrix_b1); // zet 1 op matrix
  }
  if (toestand == DRAAIEN) {
    tt_matrix.SetAllPWM((uint8_t *)matrix_b2); // zet 2 op matrix
    Serial1.printf("[TELLO] %s", "cw 90");     // draai stuk, code uitvoeren gaat door terwijl drone draait
  }
  if (toestand == LANDEN) {
    tt_matrix.SetAllPWM((uint8_t *)matrix_b3); // zet 3 op matrix
  }
}