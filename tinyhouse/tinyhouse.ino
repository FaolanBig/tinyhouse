/////////////////////
/// input devices ///
/////////////////////

#define LDR_PIN A0
#define LDR_THRESHOLD 500
#define LDR_MAX 1023
#define LDR_MIN 0
#define LDR_SCALE 100.0
#define LDR_READ_INTERVAL 1000
#define SW_PIN 2
#define DHT_PIN 4
#define DHT_TYPE DHT11

//////////////////////
/// output devices ///
//////////////////////

// #define H_BRIDGE_IN1 2
// #define H_BRIDGE_IN2 3
// #define H_BRIDGE_EN 8
// #define H_BRIDGE_IN3 9
#define MOTOR_PIN 3
#define MOTOR_SPEED 255

#define FAN_PIN 5
#define FAN_SPEED_HIGH 255
#define FAN_SPEED_LOW 63
#define FAN_OFF 0
#define TEMP_THRESHOLD 25

#include <DHT.h>

DHT dht(DHT_PIN, DHT_TYPE);

//////////////////////////
/// volatile variables ///
//////////////////////////

volatile bool sw_state = LOW;
volatile bool sw_state_pending = LOW; // manually set to HIGH when the switch is pressed, and reset to LOW in the main loop after processing the switch press


void setup() 
{
  attachInterrupt(digitalPinToInterrupt(SW_PIN), []() {
    sw_state = HIGH;
    sw_state_pending = HIGH;
    }, RISING);
  attachInterrupt(digitalPinToInterrupt(SW_PIN), []() {
    sw_state = LOW;
    }, FALLING);

  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);
  // pinMode(H_BRIDGE_IN1, OUTPUT);
  // pinMode(H_BRIDGE_IN2, OUTPUT);
  // pinMode(H_BRIDGE_EN, OUTPUT);
  // pinMode(H_BRIDGE_IN3, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  dht.begin();
}

void loop()
{
  // read the LDR value and print it to the serial monitor
  int ldr_value = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(ldr_value);

  // read the DHT sensor values and print them to the serial monitor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // control the fan based on the temperature
  if (temperature > TEMP_THRESHOLD)
  {
    analogWrite(FAN_PIN, FAN_SPEED_HIGH);
    digitalWrite(MOTOR_PIN, HIGH);
  }
  else
  {
    analogWrite(FAN_PIN, FAN_OFF);
    digitalWrite(MOTOR_PIN, LOW);
  }

  // check if the switch state has changed and print it to the serial monitor
  if (sw_state_pending)
  {
    Serial.print("Switch State: ");
    Serial.println(sw_state ? "ON" : "OFF");
    sw_state_pending = LOW; // reset the pending state after processing
  }

  delay(LDR_READ_INTERVAL); // wait for a while before reading again
}