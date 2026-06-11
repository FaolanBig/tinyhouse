/////////////////////
/// input devices ///
/////////////////////

#define MAIN_REFRESH_RATE 1000

#define LDR_PIN A0
#define LDR_THRESHOLD_RISING 500
#define LDR_THRESHOLD_FALLING 300
#define LDR_MAX 1023
#define LDR_MIN 0
#define LDR_SCALE 100.0
#define SW_PIN 2
#define DHT_PIN 4
#define DHT_TYPE DHT11

#define SOLAR_VOLTAGE_PIN A1
#define SOLAR_VOLTAGE_SCALE 5.0 / 1023.0

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

#define LED_PIN 6
#define LED_BRIGHTNESS_HIGH 255
#define LED_BRIGHTNESS_LOW 63
#define LED_OFF 0

#define SERIAL_BAUD_RATE 9600

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

  Serial.begin(SERIAL_BAUD_RATE);
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
  int sw_value = digitalRead(SW_PIN);

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

  if (ldr_value > LDR_THRESHOLD_RISING && sw_state == HIGH) { analogWrite(LED_PIN, LED_BRIGHTNESS_HIGH); }
  else if (ldr_value < LDR_THRESHOLD_FALLING && sw_state == HIGH) { analogWrite(LED_PIN, LED_BRIGHTNESS_LOW); }
  else { analogWrite(LED_PIN, LED_OFF); }
  
  if (sw_state == LOW && sw_state_pending != sw_state) { sw_state_pending = LOW; }
  
  delay(MAIN_REFRESH_RATE);
}