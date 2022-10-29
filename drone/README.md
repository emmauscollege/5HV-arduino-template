# tello-talent

## Inleiding
Tello Talent bestaat uit twee onderdelen: de educatieve Tello drone en een extra programmeerbaar element, de RMTT (remote Tello Talent o.i.d.). De educatieve Tello is in zichzelf al via telemetrie te besturen. Hiervoor moet je bijv. met een laptop verbinden met het WiFi SSID van de Tello en met een app die UDP packets over poort 8889 kan zenden de berichten versturen. De SDK waarmee de Tello bestuurd kan worden is [hier](https://dl.djicdn.com/downloads/RoboMaster+TT/Tello_SDK_3.0_User_Guide_en.pdf) te vinden. Deze bestanden zijn ook aan dit repository toegevoegd.

De RMTT kan deze commando's ook via de seriële ingang van de Tello verzenden, met toevoeging van het prefix '[TELLO] '. Antwoorden komen terug met het prefix ETT.

Onze Tello werd geleverd met verouderde firmware, waardoor we in eerste instantie de voorbeeldprogramma's niet allemaal aan de praat kregen. De Tello kan worden geupdate met de laatste firmware via de Tello app uit de appstore (bij GEE op zijn telefoon werkt deze app niet). Leerlingen krijgen de drone in principe met een werkende firmware versie, dus die hoeven zich hier geen zorgen over te maken. Onze Tello is geupdate naar firmware versie 02.05.01.19 (dit is de meest actuele versie op 9 juli 2022).

## Tello sensors
De Tello heeft een frontfacing camera die een videostream uit uitvoeren via UDP poort 11111.

De Tello heeft ook een downfacing camera voor het herkennen van mission pads. Deze mission pads helpen bij positionering. Je kunt de Tello naar een mission pad sturen en dan een x y z coördinaat van -500 - 500 geven. Onbekend is wat de absolute verplaatsing t.o.v. die missionpads is.

De Tello heeft ook een (infrarood?)sensor voor de afstand tot de grond. Onbekend is nog hoe / of deze uitgevraagd kan worden.

## RMTT programmeren
Je sluit de RMTT op de Tello aan met het zwarte kabeltje. De RMTT bevat een ESP32 controller die los programmeerbaar is. Op het moment dat de RMTT op de Tello wordt aangesloten, verandert het SSID van 'TELLO.....' naar 'RMTT.....'. Het lijkt erop dat de TELLO zijn WiFi op dat moment uitzet en de RMTT het SSID presenteert. Dit heb ik echter nog niet met zekerheid kunnen terugvinden. Het zou kunnen dat het nog steeds de TELLO is die het WiFi netwerk presenteert, maar met een andere SSID.

De RMTT is te programmeren met het Arduino framework. Hiervoor zijn libraries beschikbaar, zie [hier](https://github.com/RoboMaster/RMTT_Libs). Deze libaries zijn ook aan dit repo toegevoegd. 

De RMTT heeft een RGB-led aan de bovenkant (pins 32, 33, 25) en een drukknop (pin 34) aan de zijkant. Deze zijn los aan te sturen of uit te lezen, evt. via de RMTT-libraries. De RMTT heeft ook een connector waarop een meegeleverd accessoire kan worden aangesloten of andere, zelfgemaakte hardware. Hiervoor is bij de drone een los printje meegeleverd die de connector omzet naar (holes voor)dupont pins met een 2.54 spacing. Het meegeleverde accessoire is een 8x8 matrix met RGB leds met daarbovenop een TOF LIDAR afstandsmeter (de VL53L0X) die metingen kan doen tot 2 meter en waarmee je 1D-gebaren kunt detecteren.

## Arduino IDE voorbereiden
Voor het programmeren van de RMTT moeten eerst ESP32 boards worden toegevoegd aan de Arduino IDE:
- Ga naar de voorkeuren van het programma en vul deze URL in bij Additional Board Manager URLs: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
- Zoek in de Board Manager naar 'esp32' en installeer deze boards. De Board Manager vind je in de Arduino IDE in het menu Hulpmiddelen -> Board:... -> Board Beheer. Zoek in het window dat verschijnt op esp32, er verschijnt 1 optie, druk op installeren. Druk op sluiten als de installatie gereed is.
- Kies het board 'ESP32 WRover Kit (all versions)', je vindt dit in het menu "Hulpmiddelen" -> "Board:..." -> "ESP32 Arduino" -> "ESP32 WRover Kit (all versions)".
- In het menu Hulpmiddelen zijn nu een aantal extra opties toegevoegd om instellingen te doen. Kies de volgende instellingen:
  - Upload speed: "921600"
  - CPU Frequency: "240MHz (WiFi/BT)" --> onzeker over noodzaak van deze instelling, is misschien niet helemaal correct
  - Flash Frequency: "40MHz"
  - Flash Mode: "DIO"
  - Flash Size: "2MB (16Mb)"
  - Partition Scheme: "Minimal (1.3MB APP/700KB SPIFFS)"
- Bij het aansluiten is de RMTT onder 'Poort' te vinden als 'usbserialxxx'.
- De default serial port baudrate is 115200. Het is raadzaam dit standaard aan te houden, ook in eigen seriële communicatie omdat fouten ook met die baudrate worden gecommuniceerd.

## RMTT library installeren
De library heeft een aantal classes waarmee de verschillende componenten van de RMTT **en** de communicatie met de Tello gemakkelijk geregeld kunnen worden:
- RMTT_Protocol -> biedt een interface om gemakkelijk te communiceren met de Tello drone
- RMTT_Matrix -> hiermee kun je gemakkelijk de RGB-led matrix besturen
- RMTT_RGB -> class die gemakkelijke besturing voor de RGB-led bovenop biedt
- RMTT_TOF -> interface voor de LIDAR sensor, afkomstig van [https://github.com/pololu/vl53l0x-arduino]
- RMTT_Libs -> bovenstaande (en meer) classes in één keer importeren

De library biedt meer, maar de exacte werking daarvan is niet altijd even duidelijk.

LET OP: de libary levert ook voorbeelden, maar deze voorbeelden maken niet overal gebruik van alle functies. Soms is de code veel minder mooi dan met de aangeleverde functies zou kunnen. Kortom: een beetje een rommeltje.

Voordat je de library kunt gebruiken, moet je deze installeren in de Arduino IDE. Op de schoolcomputers moet dat elke keer als je opnieuw inlogt, op je eigen computer hoeft dat maar één keer. Installeren gaat als volgt:
- Download de library in zip-formaat (RMTTlib.zip) uit de repo. Een Macbook pakt zip-bestanden automatische uit, je moet daarom na downloaden het zip-bestand weer inpakken. Dit doe je door met je rechtmuisknop op de map RMTTlib (in Downloads) te klikken en in het menu dat verschijnt te kiezen voor "Comprimeer RMTTlib".
- Kies de Arduino IDE het menu "Schets" -> "Bibliotheek gebruiken" -> "Voeg .ZIP bibliotheek toe" -> kies het bestand RMTTlib.zip
- Als het gelukt is, dan zie je RMTTlib staan in de Arduino IDE bij het menu "Bestand" -> "Voorbeelden"

## Je eerste programma in de drone zetten
- Sluit de RMTT met het zwarte kabeltje aan op de drone
- Sluit een kabel aan tussen je computer en de RMTT
- Kies "Bestand" -> "Voorbeelden" -> "Tororial" -> "led"
- Kies in het menu "Schets" -> "Upload"
- Je ziet de melding "Schets aan het compileren" en vervolgens "Schets aan het uploaden" onder de code verschijnen. Daaronder verschijnen meldingen hoe het verloopt (of errrors). Als het goed is gegaan dan is de laatste melding:
```
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
```
- Het lampje bovenop de RTMM gaat aan en uit als een soort hartslag.
- Als je de Tello aanzet met de rode knop aan de zijkant, dan krijgt de RMTT stroom uit de batterij in de Tello. Je kunt de kabel met de computer dan eruit halen, terwijl het programma draait. 
- Als je RMTT stopt op het moment dat je de stekker van de computer eruit trekt, dan staat de Tello uit of is de batterij van de Tello leeg. Het rode lichtje op de Tello knippert rood als de batterij bijna op is. De RMTT onthoudt het programma dat je erin hebt gestopt, ook als je de stroom eraf is geweest. 

## Je drone laten vliegen met een programma
- Zet je drone op een veilige plaats, hij gaat ongeveer een meter de lucht in land dan weer.
- Zet de stand van de schakelaar op de RTMM omlaag, op het telefoon icoontje
- Sluit de RMTT met het zwarte kabeltje aan op de drone
- Zet de Tello aan met de rode knop op de zijkant
- Sluit een kabel aan tussen je computer en de RMTT
- Kies "Bestand" -> "Voorbeelden" -> "Tororial" -> Kies het voorbeeldprogramma "io_ctrl".
- Na het uploaden van je programma knippert het lampje op je tello paars als het programma op de RTMM verbinding heeft gemaakt met de Tello, het knippert oranje als er geen verbinding is.
- Als er geen verbinding is, terwijl je programma de Tello wel zou moeten laten vliegen: zet de stand van de schakelaar op de RTMM omlaag op het telefoon icoontje als je dat eerder vergeten was, verwijder de kabel tussen PC en drone, zet de tello uit en weer aan met de rode knop aan de zijkant van de tello. Wacht ongeveer 10 seconden, het ledje zou nu paars moeten knipperen.
- Druk op het knopje aan de zijkant van de RMTT, je Tello gaat vliegen nadat je deze knop hebt ingedrukt.

