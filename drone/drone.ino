/*****************************************
   Drone Startcode
   met toestansdiagrammen
   Emmauscollege
   v20230921CAM
 *****************************************/

/*****************************************
   libraries die je gebruikt 

   Libraries van de fabrikant van de Tello drone
   https://github.com/RoboMaster/RMTT_Libs

 *****************************************/

#include <Wire.h>
#include "src/RMTT_Libs/RMTT_Libs.h"


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
const int DRAAIEN   = 2; // speel het spel
const int LANDEN    = 3; // laat zien wie de winnaar is
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
     Serial.println("ERROR: Failed to detect and initialize TOF sensor!");
     while (1) {}
  }
  
  // tello initialiseren
  Serial.begin(115200); // verbinding tussen RMTT en computer starten
  Serial.println("Serial init done");
  Serial1.begin(1000000, SERIAL_8N1, 23, 18); // verbinding tussen RMTT en drone starten
  tello.startUntilControl(); // wacht tot Tello gereed is om te starten en knopje op RMTT is ingedrukt
  Serial.println("Tello init done");         // stuur debug-bericht naar serial monitor
  // tello laten vliegen. 
  // Er zijn allerlei commando's die de drone naar een bepaalde plek sturen, zie de SDK 3.0 documentatie uit de repo. 
  // Die plek is wel onnauwkeurig.
  // Als de drone een mission pad herkent, dan kun je de drone naar een plek sturen ten opzichte van het mission pad 
  // (in plaats van ten opzichte van opstijgen), dat helpt met de nauwkeurigheid, 
  // in deze voorbeeldcode gebruiken we geen mission pads

  // init eerste toestand
  unsigned long toestandStartTijd = millis();
  Serial1.printf("[TELLO] %s", "takeoff"); // start met opstijgen, uitvoeren van code gaat door terwijl opstijgen bezig is.
  Serial.print("TT drone command:");Serial.println("takeoff"); // stuur debug-bericht naar serial monitor

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

  // lees sensorwaarden
  response = "timeout";
  response = tello.getTelloResponseString(100); // response is "timeout" als na 100 ms nog geen bericht is ontvangen (de drone is dan nog bezig met de laatste opdracht)
  if (response != "timeout") {Serial.print("TT drone response:");Serial.println(response);}
  // response is timeout (niks beschikbaar), ETT ok (commando uitgevoerd) of ETT ... (er is iets mis gegaan)
  // zie SDK documentatie voor meer informatie over response

  afstand = tt_sensor.ReadRangeSingleMillimeters(); // afstand in mm, sensor leest tot ongeveer 1000 mm, 8192 betekent buiten range
  if (tt_sensor.TimeoutOccurred()) {Serial.println("ERROR: timeout reading TOF sensor");}

  // bepaal toestand
  if (toestand == OPSTIJGEN) {
    if (response == "ETT ok") { // drone klaar met opstijgen
      toestand = DRAAIEN;
      Serial.println("Nieuwe toestand: DRAAIEN");
      Serial1.printf("[TELLO] %s", "cw 90");     // draai stuk, code uitvoeren gaat door terwijl drone draait
      Serial.print("TT drone command:");Serial.println("cw 90");  // stuur debug-bericht naar serial monitor
    }
  }
  else if (toestand == DRAAIEN) {
    if (response == "ETT ok" && // drone heeft 90 graden gedraaid
        afstand < 500) { // afstandssensor ziet iets op minder dan 50 cm (bijvoorbeeld een hand)
      // het stop commando zou moeten werken, maar de drone geeft als response "ETT unknown command: stop[TELLO]"
      // daarom maar gekozen voor een minder ideale oplossing waarbij de drone steeds een stukje draait.
      // Serial1.printf("[TELLO] %s", "stop"); // vorige commando (draaien) waarmee de drone bezig is wordt afgebroken
      // Serial.print("TT drone command:");Serial.println("stop"); // stuur debug-bericht naar serial monitor
      toestand = LANDEN;
      Serial.println("Nieuwe toestand: LANDEN");
      Serial1.printf("[TELLO] %s", "land"); // stuur commando landen, code uitvoeren gaat door terwijl drone landt 
      Serial.print("TT drone command:");Serial.println("land"); // stuur debug-bericht naar serial monitor
    } 
    else if (response == "ETT ok" ) { // verder draaien, want niks dichter bij dan 500 mm
      Serial1.printf("[TELLO] %s", "cw 90");     // draai stuk, code uitvoeren gaat door terwijl drone draait
      Serial.print("TT drone command:");Serial.println("cw 90");  // stuur debug-bericht naar serial monitor
    }
  }
  else if (toestand == LANDEN) {
    // drone blijft in deze toestand tot reset
  }

  // zet actuatoren
  // hier liever geen vlieg-opdrachten zetten, 
  // want doordat loop() steeds opnieuw wordt aangeroepen en vlieg-opdrachten veel tijd kosten, 
  // bouwt de drone een queue (wachtrij) op van vliegopdrachten 
  // daardoor gaat de uitvoering van vliegopdrachten achterlopen op de toestand waar de drone inzit
  if (toestand == OPSTIJGEN) {
    tt_matrix.SetAllPWM((uint8_t *)matrix_b1); // zet 1 op matrix
  }
  else if (toestand == DRAAIEN) {
    tt_matrix.SetAllPWM((uint8_t *)matrix_b2); // zet 2 op matrix
  }
  else if (toestand == LANDEN) {
    tt_matrix.SetAllPWM((uint8_t *)matrix_b3); // zet 3 op matrix
  }
}
