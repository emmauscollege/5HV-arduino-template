/*****************************************
   Alphabot Startcode
   met toestansdiagrammen
   Emmauscollege
   v20221001GEE
 *****************************************/

/*****************************************
   variabelen 
 *****************************************/
// gebruikte pinnen
const int pin1Rood   = 28; // pin van stoplicht 1 rood
const int pin1Oranje = 30; // pin van stoplicht 1 oranje
const int pin1Groen  = 32; // pin van stoplicht 1 groen

// variabelen om waarden van sensoren en actuatoren te onthouden
int afstandVoor = 0;
int knop8 = 0;

// variabelen voor de toestanden
const int VOORUIT = 1; // rij vooruit
const int DRAAI = 2; // draai
int toestand = VOORUIT;
unsigned long toestandStartTijd = 0;

/*****************************************
   functies 
 *****************************************/


/*****************************************
   setup() en loop()
 *****************************************/

void setup() {                                      // deze functie wordt 1x uitgevoerd als de arduino start
  // enable console en stuur opstartbericht
  Serial.begin(9600);
  Serial.println("Alphabot start");

  // zet pinmodes voor
  pinMode(pin1Rood, OUTPUT);
  pinMode(pin1Oranje, OUTPUT);
  pinMode(pin1Groen, OUTPUT);
  
}

void loop() {                                       // deze functie wordt oneindig herhaalt
  // lees sensorwaarden
  afstandVoor = digitalRead(pin7Knop);
  Serial.print("afstandVoor: ");Serial.println(afstandVoor);

  // bepaal toestand
  if (toestand == VOORUIT) {                        // als toestand VOORUIT is
    if (afstandVoor < 50) {                         // afstand < 50 cm
      toestandStartTijd = millis();                 // reset tijd
      toestand = DRAAI;                             // zet nieuwe toestand
      Serial.println("Nieuwe toestand: DRAAI");
    }
    if (false) {                                    // conditie voor 2e pijl uit toestand VOORUIT invullen
                                                    // code invullen
    }
  }
  if (toestand == DRAAI) {                          // als toestand DRAAI is
    if (millis() - toestandStartTijd > 1000) {      // 1 seconde voorbij
      toestandStartTijd = millis();                 // reset tijd
      toestand = VOORUIT;                           // zet nieuwe toestand
      Serial.println("Nieuwe toestand: VOORUIT");
    }
  }

  // zet actuatoren zoals hoort bij de toestand
  if (toestand == VOORUIT) {
    for (int i = 1; i <= 8; i = i + 1) {
      stoplicht(i, ROOD);
    }
  }
  if (toestand == DRAAI) {
    for (int i = 1; i <= 8; i = i + 1) {
      stoplicht(i, GROEN);
    }
  }

  // vertraging om te zorgen dat berichten op de seriele monitor leesbaar blijven
  delay(50);
}
