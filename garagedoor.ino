/*
 * Hall effect code courtesy of Rob van den Tillaart and Ralph Martin, http://playground.arduino.cc/Code/HallEffect
 * This is going to use a hall effect sensor to detect two opposed magnets, one at top of garage door and one at bottom,
 * one in north orientation and the other in south, so that it will know when garage door is fully open and fully closed.
 * It will also be able to toggle a relay that will open / close the garage door.  In the future, it will also be able to
 * detect when garage door is in motion by deting current draw of the garage door itself, being a passive ammeter.
 */

#define RELAYPIN 4
#define TOGGLETIME 5000 // time to toggle the garage door switch in ms

#define NOFIELD 512L    // Analog output with no applied field, calibrate this

int led_pin[6]={13, 9, 10, 3, 5, 6};
int led_state[6]={};

// Uncomment one of the lines below according to device in use A1301 or A1302
// This is used to convert the analog voltage reading to milliGauss
#define TOMILLIGAUSS 1953L  // For A1301: 2.5mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 1953mG
// #define TOMILLIGAUSS 3756L  // For A1302: 1.3mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 3756mG


void setup() {
  Serial.begin(9600);
  pinMode(RELAYPIN, OUTPUT);
}

long DoMeasurement() {
// measure magnetic field
  int raw = analogRead(0);   // Range : 0..1024

//  Uncomment this to get a raw reading for calibration of no-field point
  // Serial.print("Raw reading: ");
  // Serial.println(raw);

  long compensated = raw - NOFIELD;                 // adjust relative to no applied field 
  long gauss = compensated * TOMILLIGAUSS / 1000;   // adjust scale to Gauss
  return gauss;
}

void loop() {
    
    if (Serial.available() > 0) {
        char command = Serial.read();
        switch(command) {
            case '1':
                Serial.println(DoMeasurement(), 10);
                break;
            case '2':
                relayHigh();
                delay(TOGGLETIME);
                relayLow();
                Serial.println(F("ok"));
                break;
            default:
                printHelp();
        }
    }
    
}

void relayHigh() {
    digitalWrite(RELAYPIN, HIGH);
}

void relayLow() {
    digitalWrite(RELAYPIN, LOW);
}

void printHelp() {
    Serial.println(F("Enter one of hte following commands:"));
    Serial.println(F("1 - read hall sensor, prints reading in gauss, positive or negative"));
    Serial.println(F("2 - toggle garage door open closed"));
}
