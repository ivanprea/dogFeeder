#include <Servo.h>
#include <LiquidCrystal.h>

// Define the pins for the tilt sensor, LEDs, ultrasonic sensor, and servo motor
const int tiltSensorPin = 2;        // Digital pin connected to tilt sensor
const int greenLedPin = 3;          // Pin connected to green LED
const int redLedPin = 4;            // Pin connected to red LED
const int ultrasonicTrigPin = 5;    // Ultrasonic sensor TRIG pin
const int ultrasonicEchoPin = 6;    // Ultrasonic sensor ECHO pin
const int servoPin = 7;             // Servo motor signal pin
const int whiteLedPin = A1;         // Pin connected to white LED
const int contrastPin = A0;         // Potentiometer connected to adjust contrast
const int blueLedPin = A2;          // Pin connected to blue LED
const int redAlertPin = A3; // Define pin A3 for red alert LED

// Define LCD pins
const int rs = 8;
const int en = 9;
const int d4 = 10;
const int d5 = 11;
const int d6 = 12;
const int d7 = 13;

// LCD dimensions (16x2 for a standard 16x2 LCD)
const int lcdColumns = 16;
const int lcdRows = 2;

// Create instances of libraries
Servo servo;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool ultrasonicEnabled = true; // Flag to indicate if ultrasonic is enabled
bool ultrasonicTriggered = false; // Flag to indicate if ultrasonic has been triggered
unsigned long ultrasonicLastTriggerTime = 0; // Timestamp for when ultrasonic was last triggered
unsigned long servingStartTime = 0; // Timestamp for when serving started
bool isAlertDisplayed = false; // Flag to indicate if alert message is displayed

void setup() {
  // Initialize the tilt sensor, LEDs, ultrasonic sensor, and servo motor
  pinMode(tiltSensorPin, INPUT_PULLUP); // Using internal pull-up resistor
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);
  pinMode(whiteLedPin, OUTPUT); // Initialize white LED pin
  pinMode(blueLedPin, OUTPUT); // Initialize blue LED pin
  pinMode(servoPin, OUTPUT); // Initialize servo pin
  
  // Attach servo to its pin
  servo.attach(servoPin);

  // Initialize the LCD with the specified dimensions
  lcd.begin(lcdColumns, lcdRows);

  // Display a message
  lcd.print("DOG FEEDER  v1.7");

  // Set up the contrast pin as an output
  pinMode(contrastPin, OUTPUT);

  // Ensure the servo is at 0 degrees on startup
  servo.write(0);

  // Turn on blue LED
  digitalWrite(blueLedPin, HIGH);

  // Define red alert pin as output
  pinMode(redAlertPin, OUTPUT);
}

void loop() {
  // Check if sensor is tilted up (on)
  int sensorValue = digitalRead(tiltSensorPin);
  if (sensorValue == LOW) {
    digitalWrite(greenLedPin, HIGH); // Turn on green LED
    digitalWrite(redLedPin, LOW);    // Turn off red LED
    isAlertDisplayed = false;
    
    // Read ultrasonic sensor distance
    long duration, distance;
  
    // Enable or disable ultrasonic sensor based on ultrasonicEnabled flag
    if (ultrasonicEnabled) {
      digitalWrite(ultrasonicTrigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(ultrasonicTrigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(ultrasonicTrigPin, LOW);
      duration = pulseIn(ultrasonicEchoPin, HIGH);
      distance = duration * 0.034 / 2;
    } else {
      distance = 999; // Set distance to a large value if ultrasonic is disabled
    }
    
    // Read the value from the contrast adjustment potentiometer
    int contrastValue = analogRead(contrastPin);
    analogWrite(contrastPin, contrastValue / 4); // Adjust contrast using PWM
    
    // Enable ultrasonic sensor if it's not currently enabled and no object has been detected
    if (!ultrasonicTriggered && distance >= 10) {
      ultrasonicTriggered = true;
      ultrasonicLastTriggerTime = millis(); // Record the time when ultrasonic was last triggered
      ultrasonicEnabled = true; // Enable ultrasonic sensor
      servingStartTime = millis(); // Record the time when serving started
    }
    
    // If ultrasonic is triggered and object detected within 10 cm, move servo to 90 degrees
    if (ultrasonicTriggered && distance < 10) {
      servo.write(90); // Move servo to 90 degrees
      digitalWrite(whiteLedPin, HIGH); // Turn on white LED
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Serving  portion");
      lcd.setCursor(4, 1);
      lcd.print("of 110g");
    
      // Delay for 1.5 seconds after object detection (Servo stays open for 1.5 seconds)
      delay(1500);
    
      // Disable ultrasonic sensor after 2 seconds
      ultrasonicTriggered = false; // Reset the ultrasonic trigger flag
      ultrasonicEnabled = false; // Disable ultrasonic sensor
      servo.write(0); // Move servo to 0 degrees
      digitalWrite(whiteLedPin, LOW); // Turn off white LED
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Take your bowl"); // Display "Buon appetito!" message
      lcd.setCursor(1, 1);
      lcd.print("Buon appetito!"); // Display "Buon appetito!" message
      // Delay for 2.5 seconds before reverting to the initial message
      delay(2500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Place your bowl.");
    } else {
      // If serving and 1.8 seconds have passed, close the servo
      if (ultrasonicTriggered && millis() - servingStartTime >= 1800) {
        ultrasonicTriggered = false; // Reset the ultrasonic trigger flag
        ultrasonicEnabled = false; // Disable ultrasonic sensor
        servo.write(0); // Move servo to 0 degrees
        digitalWrite(whiteLedPin, LOW); // Turn off white LED
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Place your bowl.");
      }
    }
    
    // If ultrasonic has been triggered and 3000 ms have passed since the last trigger, enable ultrasonic sensor
    if (ultrasonicTriggered && millis() - ultrasonicLastTriggerTime >= 3000) {
      ultrasonicEnabled = true; // Enable ultrasonic sensor
    }
    
    // Turn off blue LED if red LED is flickering
    if (isAlertDisplayed) {
      digitalWrite(blueLedPin, LOW); // Turn off blue LED
    } else {
      digitalWrite(blueLedPin, HIGH); // Turn on blue LED
    }
    
    delay(100); // Adjust delay as needed
  } else {
    digitalWrite(greenLedPin, LOW);   // Turn off green LED
    digitalWrite(redLedPin, HIGH);   // Turn on red LED
    
    if (!isAlertDisplayed) {
      // Display alert message
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("WARNING!");
      lcd.setCursor(1, 1);
      lcd.print("Put more food.");
      
      isAlertDisplayed = true;
    }
    
    // Flicker the red LED when the system is off temporarily
    for (int i = 0; i < 5; i++) {
      digitalWrite(redAlertPin, HIGH); // Turn on the red LED
      delay(700); // Delay for flickering effect
      digitalWrite(redAlertPin, LOW); // Turn off the red LED
      delay(300); // Delay for flickering effect
      digitalWrite(redAlertPin, HIGH); // Turn off the red LED
      delay(700); // Delay for flickering effect
      digitalWrite(redAlertPin, LOW); // Turn off the red LED
      delay(1000); // Delay for flickering effect
    }
    
    // Turn off blue LED when system is down
    digitalWrite(blueLedPin, LOW); // Turn off blue LED
    
    // Add any additional actions you want to perform when the system is down
  }
}
