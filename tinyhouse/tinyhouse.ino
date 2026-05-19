/////////////////////
/// input devices ///
/////////////////////

#define LDR_PIN A0

#define DHT_PIN 4
#define DHT_TYPE DHT11

//////////////////////
/// output devices ///
//////////////////////

// #define H_BRIDGE_IN1 2
// #define H_BRIDGE_IN2 3
// #define H_BRIDGE_EN 8
// #define H_BRIDGE_IN3 9
#define M_SWITCH_PIN 3
#define MotorSpeed 255

#define FAN_PIN 2
#define FAN_SPEED 255
#define TEMP_THRESHOLD 25

#include <DHT.h>

DHT dht(DHT_PIN, DHT_TYPE);

void setup() 
{
  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);
  // pinMode(H_BRIDGE_IN1, OUTPUT);
  // pinMode(H_BRIDGE_IN2, OUTPUT);
  // pinMode(H_BRIDGE_EN, OUTPUT);
  // pinMode(H_BRIDGE_IN3, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(M_SWITCH_PIN, OUTPUT);
  dht.begin();
}

void loop()
{
    
}