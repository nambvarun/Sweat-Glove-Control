// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <Servo.h>

#define DHT_PIN 2
#define SERVO_PIN 3
#define TEMP_PIN 0

// Uncomment whatever type you're using!
#define DHT_TYPE DHT22   // DHT 22  (AM2302), AM2321

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
DHT dht(DHT_PIN, DHT_TYPE);
Servo servo;
int servo_pos = 0;
bool ventIsOpen = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Glove Test!");

  dht.begin();
  servo.attach(SERVO_PIN);
  servo_pos = 30;
  servo.write(servo_pos);
//  analogReference(INTERNAL);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();    // temp in celsius
  float f = dht.readTemperature(true);  // temp in fahrenheit

  int input_bits = analogRead(TEMP_PIN);
  float m_volts = (input_bits / 1024.0) * 5000;
  float celsius = m_volts / 10;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default) and Celsius
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  if (h > 50 && !ventIsOpen) {
    for (servo_pos = 30; servo_pos <= 150; servo_pos++)  {
      servo.write(servo_pos);
      delay(15);
    }
    ventIsOpen = true;
  } else if (h < 50 && ventIsOpen)  {
    for (servo_pos = 150; servo_pos >= 30; servo_pos--)  {
      servo.write(servo_pos);
      delay(15);
    }
    ventIsOpen = false;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Servo Pos: ");
  Serial.print(servo_pos);
  Serial.println("");
  Serial.print("Humidity Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.println(" *F\t");
  Serial.print("TMP35 CAZ\t");
  Serial.print(celsius);
  Serial.print(" *C, ");

  Serial.print((celsius * 9) / 5 + 32);
  Serial.println(" *F");
  //  Serial.print("Heat index: ");
  //  Serial.print(hic);
  //  Serial.print(" *C ");
  //  Serial.print(hif);
  //  Serial.println(" *F");
}
