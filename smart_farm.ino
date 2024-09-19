//© Smart Farm Project 2020 - HRCC

// Pin Definitions
int TEMP_PIN = 0;      // Temperature sensor (DHT11)
int HUM_PIN = 1;       // Humidity sensor (DHT11)
int RELAY_TEMP = 3;    // Relay for Temperature control
int RELAY_HUM = 4;     // Relay for Humidity control
int RELAY_WATER = 13;  // Relay for Water Pump
int BUZZER = 9;        // Buzzer for Bluetooth testing

// LED Indicators
int TEMP_HIGH_LED = 6;  // High temperature LED
int TEMP_LOW_LED = 7;   // Low temperature LED
int HUM_HIGH_LED = 8;   // High humidity LED
int HUM_LOW_LED = 5;    // Low humidity LED
int MOISTURE_HIGH_LED = 10; // High moisture LED
int MOISTURE_LOW_LED = 11;  // Low moisture LED

// Soil Moisture Sensors
const int PROBE1 = 0;
const int PROBE2 = 1;
const int PROBE3 = 2;
const int PROBE4 = 3;
const int PROBE5 = 4;

// Moisture Variables
int moistureLevel1, moistureLevel2, moistureLevel3, moistureLevel4, moistureLevel5;
float averageMoisture;

// Temperature Variables
float R1 = 10000, logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int Vo;
int SET_TEMP = 27;     // Set temperature threshold
int SET_HUM = 30;      // Set humidity threshold
int humidityLevel;

// Bluetooth Module Pins
const int BLUETOOTH_RX = 10;
const int BLUETOOTH_TX = 11;

void setup() {
  Serial.begin(9600);

  // Pin Modes
  pinMode(RELAY_TEMP, OUTPUT);
  pinMode(RELAY_HUM, OUTPUT);
  pinMode(RELAY_WATER, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // LEDs
  pinMode(TEMP_HIGH_LED, OUTPUT);
  pinMode(TEMP_LOW_LED, OUTPUT);
  pinMode(HUM_HIGH_LED, OUTPUT);
  pinMode(HUM_LOW_LED, OUTPUT);
  pinMode(MOISTURE_HIGH_LED, OUTPUT);
  pinMode(MOISTURE_LOW_LED, OUTPUT);

  digitalWrite(RELAY_TEMP, LOW);
  digitalWrite(RELAY_HUM, LOW);
  digitalWrite(RELAY_WATER, LOW);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  // Read temperature and humidity
  Vo = analogRead(TEMP_PIN);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  T = T - 273.15;
  T = (T * 9.0) / 5.0;  // Convert Kelvin to Fahrenheit
  Serial.print("Temperature: ");
  Serial.print(T);
  Serial.println(" C");

  // Humidity Calculation
  int H = analogRead(HUM_PIN);
  humidityLevel = H * 100 / 1023;
  Serial.print("Humidity Level: ");
  Serial.println(humidityLevel);

  // Soil Moisture Calculation
  moistureLevel1 = map(analogRead(PROBE1), 1023, 90, 0, 100);
  moistureLevel2 = map(analogRead(PROBE2), 1023, 90, 0, 100);
  moistureLevel3 = map(analogRead(PROBE3), 1023, 90, 0, 100);
  moistureLevel4 = map(analogRead(PROBE4), 1023, 90, 0, 100);
  moistureLevel5 = map(analogRead(PROBE5), 1023, 90, 0, 100);
  averageMoisture = (moistureLevel1 + moistureLevel2 + moistureLevel3 + moistureLevel4 + moistureLevel5) / 5;
  Serial.print("Average Soil Moisture: ");
  Serial.println(averageMoisture);

  // Relay Control for Temperature
  if (T > SET_TEMP) {
    digitalWrite(RELAY_TEMP, HIGH);  // Turn on fan or cooling
    digitalWrite(TEMP_HIGH_LED, HIGH);  // High temperature LED on
    digitalWrite(TEMP_LOW_LED, LOW);   // Low temperature LED off
  } else {
    digitalWrite(RELAY_TEMP, LOW);
    digitalWrite(TEMP_HIGH_LED, LOW);  // High temperature LED off
    digitalWrite(TEMP_LOW_LED, HIGH);  // Low temperature LED on
  }

  // Relay Control for Humidity
  if (humidityLevel > SET_HUM) {
    digitalWrite(RELAY_HUM, HIGH);  // Turn on dehumidifier
    digitalWrite(HUM_HIGH_LED, HIGH);  // High humidity LED on
    digitalWrite(HUM_LOW_LED, LOW);   // Low humidity LED off
  } else {
    digitalWrite(RELAY_HUM, LOW);
    digitalWrite(HUM_HIGH_LED, LOW);  // High humidity LED off
    digitalWrite(HUM_LOW_LED, HIGH);  // Low humidity LED on
  }

  // Relay Control for Soil Moisture
  if (averageMoisture < 50) {
    digitalWrite(RELAY_WATER, HIGH);  // Turn on water pump
    digitalWrite(MOISTURE_LOW_LED, HIGH);  // Low moisture LED on
    digitalWrite(MOISTURE_HIGH_LED, LOW);   // High moisture LED off
  } else {
    digitalWrite(RELAY_WATER, LOW);
    digitalWrite(MOISTURE_LOW_LED, LOW);  // Low moisture LED off
    digitalWrite(MOISTURE_HIGH_LED, HIGH);  // High moisture LED on
  }

  // Bluetooth Control
  if (Serial.available()) {
    char command = Serial.read();
    if (command == '1') {
      digitalWrite(RELAY_WATER, HIGH);  // Manual control to turn on water pump
    } else if (command == '0') {
      digitalWrite(RELAY_WATER, LOW);   // Manual control to turn off water pump
    } else if (command == 't') {
      digitalWrite(BUZZER, HIGH);       // Bluetooth connection test with buzzer
      delay(1000);
      digitalWrite(BUZZER, LOW);
    }
  }

  delay(1000);
}
