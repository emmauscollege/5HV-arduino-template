/*****************************************
   Game Startcode
   met toestansdiagrammen
   Emmauscollege
   v20201114GEE
 *****************************************/

/*****************************************
   libraries die je gebruikt 

   LCD met 2 regels van 16 karakters 
   (de RGB backlit kan niet van kleur worden veranderd)
   https://www.arduino.cc/reference/en/libraries/grove-lcd-rgb-backlight/

 *****************************************/
#include <Wire.h>
#include "rgb_lcd.h"

/*****************************************
   variabelen die je gebruikt maken
 *****************************************/
// gebruikte pinnen
const int pinLedBlauw   = 11; 
const int pinLedGroen   = 5; 
const int pinLedGeel    = 9; 
const int pinLedWit     = 3; 
const int pinKnopBlauw  = 10; 
const int pinKnopGroen  = 6; 
const int pinKnopGeel   = 8; 
const int pinKnopWit    = 4; 

// variabelen om waarden van sensoren en actuatoren te onthouden
int knopA = 0;
int knopB = 0;

// variabelen voor de toestanden
const int TELAF = 1; // tel af tot spel start
const int SPEEL = 2; // speel het spel
const int WIN   = 3; // laat zien wie de winnaar is
int toestand = TELAF;
unsigned long toestandStartTijd = 0;

/*****************************************
   functies die je gebruikt maken
 *****************************************/
// code die steeds wordt uitgevoerd in toestand TELAF
void telafLoop() {
  // tel af
}

// code die steeds wordt uitgevoerd in toestand SPEEL
void speelLoop() {
  // speel spel
}

// code die steeds wordt uitgevoerd in toestand SPEEL
void winLoop() {
  // toon wie gewonnen heeft
}


/*****************************************
   setup() en loop()
 *****************************************/

void setup() {
  // enable console en stuur opstartbericht
  Serial.begin(9600);
  Serial.println("Game start");

  // zet pinmode voor leds
  pinMode(pinLedBlauw, OUTPUT);
  pinMode(pinLedGroen, OUTPUT);
  pinMode(pinLedGeel, OUTPUT);
  pinMode(pinLedWit, OUTPUT);
  
  // zet pinmode voor knoppen
  pinMode(pinKnopBlauw, INPUT);
  pinMode(pinKnopGroen, INPUT);
  pinMode(pinKnopGeel, INPUT);
  pinMode(pinKnopWit, INPUT);
  
while(true) {
  if(digitalRead(pinKnopBlauw) == HIGH) {
    digitalWrite(pinLedBlauw, HIGH);
  } else {
    digitalWrite(pinLedBlauw, LOW);
  }  
  if(digitalRead(pinKnopGroen) == HIGH) {
    digitalWrite(pinLedGroen, HIGH);
  } else {
    digitalWrite(pinLedGroen, LOW);
  }  
  if(digitalRead(pinKnopGeel) == HIGH) {
    digitalWrite(pinLedGeel, HIGH);
  } else {
    digitalWrite(pinLedGeel, LOW);
  }  
  if(digitalRead(pinKnopWit) == HIGH) {
    digitalWrite(pinLedWit, HIGH);
  } else {
    digitalWrite(pinLedWit, LOW);
  }  
 }


  
  digitalWrite(pinLedGroen, HIGH);
  digitalWrite(pinLedGeel, HIGH);
  digitalWrite(pinLedWit, HIGH);
}

void loop() {
  delay(10000);
  // lees sensorwaarden
  knopA = digitalRead(pinKnopBlauw);
  knopB = digitalRead(pinKnopGroen);

  // bepaal toestand
  if (toestand == TELAF) {
    telafLoop();
    if (millis() - toestandStartTijd > 10000) { // 2 seconden voorbij
      toestandStartTijd = millis();
      toestand = SPEEL;
      Serial.println("Nieuwe toestand: SPEEL");
    }
  }
  if (toestand == SPEEL) {
    speelLoop();
    if (millis() - toestandStartTijd > 5000) { // 5 seconden voorbij
      toestandStartTijd = millis();
      toestand = WIN;
      Serial.println("Nieuwe toestand: WIN");
    }
  }
  if (toestand == WIN) {
    winLoop();
    if (millis() - toestandStartTijd > 1000 &&  // 1 seconde voorbij en
        knopA == HIGH && knopB == HIGH) {       // beide knoppen ingedrukt
      toestandStartTijd = millis();
      toestand = TELAF;
      Serial.println("Nieuwe toestand: TELAF");
    }
  }

  // zet actuatoren
  digitalWrite(pinLedBlauw, HIGH);
  

  // kleine vertraging, 100 keer per seconde loopen is genoeg
  delay(10);
}
