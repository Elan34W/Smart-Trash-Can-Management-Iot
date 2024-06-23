#define BLYNK_TEMPLATE_ID "-----Change-----"
#define BLYNK_TEMPLATE_NAME "----Change-----"
#define BLYNK_AUTH_TOKEN "-----Change-----"

// Include necessary libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "HX711.h"
#include <Servo.h>

// WiFi credentials
char ssid[] = "--Change--"; // Replace with your WiFi SSID
char pass[] = "--Change--"; // Replace with your WiFi password

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = D5;
const int LOADCELL_SCK_PIN = D6;

// Pin connections for servo
const int SERVO_PIN = D7; // Pin D3 on WEMOS D1 Mini

// Pin connections for ultrasonic sensor
const int TRIG_PIN = D1; // Pin D2 on WEMOS D1 Mini
const int ECHO_PIN = D2; // Pin D1 on WEMOS D1 Mini

// Initialize servo object
HX711 scale;
Servo myServo;

int servoAngle = 0;
int buttonStates = 0;
bool manualControl = false; // Flag to indicate manual control by button
bool binFullNotified = false; // Flag to prevent multiple notifications
bool binWeightNotified = false;
float calibration_factor = -426.77; // Use the found calibration factor

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare(); // Reset the scale to 0

  // Attach servo to specified pin
  myServo.attach(SERVO_PIN);
  
  // Initialize pins for ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Set initial servo position (0 degrees)
  myServo.write(servoAngle);
  Serial.println("Servo diatur ke posisi 0 derajat");
}

void loop() {
  Blynk.run(); // Run Blynk

  // Read weight from load cell
  if (scale.is_ready()) {
    float weight_grams  = scale.get_units(10); // Read average of 10 readings
    if (weight_grams < 0) {
      weight_grams = 0; // Set negative values to zero
    }
    // float weight_kg = weight_grams / 1000.0;  // Convert grams to kilograms
    Serial.print("Weight: ");
    Serial.print(weight_grams);
    Serial.println(" grams");

    Blynk.virtualWrite(V2, weight_grams); // Send weight to Blynk

      // Check if the bin weight is 500 and send a notification
      if (weight_grams >= 500 && !binWeightNotified) {
        Blynk.logEvent("trash_is_heavy");
        binWeightNotified = true; // Set the flag to prevent multiple notifications
      } else if (weight_grams < 500) {
        binWeightNotified = false; // Reset the flag if the bin is not full
      }

  } else {
    Serial.println("HX711 not ready.");
  }

  // Read distance from ultrasonic sensor
  long duration, distance;

  // Send trigger signal
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo signal
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1; // Convert time to distance (cm)

  // Display distance
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Send distance to Blynk virtual pin V1
  Blynk.virtualWrite(V1, distance);

  // Calculate bin level based on distance thresholds
  int binLevel = 0;
  if (distance <= 3) {
    binLevel = 100;
  } else if (distance <= 4) {
    binLevel = 75;
  } else if (distance <= 6) {
    binLevel = 50;
  } else if (distance <= 9) {
    binLevel = 25;
  } else {
    binLevel = 0;
  }

  // Send bin level to Blynk virtual pin V0
  Blynk.virtualWrite(V0, binLevel);

  // Check if the bin is full (100%) and send a notification
  if (binLevel == 100 && !binFullNotified) {
    Blynk.logEvent("trash_can_is_full");
    binFullNotified = true; // Set the flag to prevent multiple notifications
  } else if (binLevel < 100) {
    binFullNotified = false; // Reset the flag if the bin is not full
  }

  // Move servo based on detected distance only if not in manual control mode
  if (!manualControl) {
    // Move servo based on detected distance
    int servoAngle;
    if (distance < 4) {
      servoAngle = 180;
      buttonStates = 1;
      myServo.write(servoAngle); // If distance is less than 10 cm, move servo to 180 degrees
      Serial.println("Servo diatur ke posisi 180 derajat");
    } else {
      servoAngle = 0;
      buttonStates = 0;
      myServo.write(servoAngle); // If distance is 10 cm or more, move servo to 0 degrees
      Serial.println("Servo diatur ke posisi 0 derajat");
    }

    // Send servo angle to Blynk virtual pin V3
    Blynk.virtualWrite(V3, servoAngle);
    Blynk.virtualWrite(V4, buttonStates); // Sync button state
  }

  yield(); // Prevent watchdog timer reset
  delay(3000); // Update every second
}

// Blynk button handler for V4
BLYNK_WRITE(V4) {
  int buttonState = param.asInt(); // Get button state

  if (buttonState == 1) {
    if (manualControl == true) {
      manualControl = false;
    }
    else {
      manualControl = true; // Enable manual control
      servoAngle = 180; // If button is pressed, move servo to 180 degrees
      Serial.println("Button pressed, servo diatur ke posisi 180 derajat");
    }
  } else if (buttonState == 0){
    if (manualControl == false){
      manualControl = true; // Enable manual control
      servoAngle = 0; // If button is released, move servo to 0 degrees
      Serial.println("Button released, servo diatur ke posisi 0 derajat");
    }else {
      manualControl = false;
    }
  }

  myServo.write(servoAngle);
  Blynk.virtualWrite(V3, servoAngle); // Update label display with new servo angle
}
