/*
 *    FILE: MM01
 *  AUTHOR: Rob van den Tillaart; modified Ralph Martin
 *    DATE: 2012 06 10 
 *     ORIGINAL URL: http://playground.arduino.cc/Code/HallEffect
 *
 * PURPOSE: use an A1301 or A1302 as magnetometer   
 *
 * Pin Layout LH Package
 * =====================
 *  1     VCC 5V
 *  2     signal    connected to Analog 0    
 *  3     GND
 *
 * Pin Layout UA Package
 * =====================
 *  1     VCC 5V
 *  2     GND
 *  3     signal    connected to Analog 0    
 *
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
