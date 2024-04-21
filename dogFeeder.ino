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
  intLeds();
  intServo();
  intLCD();
  intTilt();
  intultraSonic();
 
}
void intultraSonic() {
  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);
}
void intTilt() {
  pinMode(tiltSensorPin, INPUT_PULLUP);
}
void intLeds() {
  // pins LED
  pinMode(greenLedPin, OUTPUT); // GREEN
  pinMode(redLedPin, OUTPUT); // RED
  pinMode(whiteLedPin, OUTPUT); // WHITE
  pinMode(blueLedPin, OUTPUT); // BLUE
  pinMode(redAlertPin, OUTPUT); // Alert RED
  // Turn on blue LED
  digitalWrite(blueLedPin, HIGH); 
}
void intServo() {
  pinMode(servoPin, OUTPUT); // Initialize servo pin
  // Attach servo to its pin
  servo.attach(servoPin);
  // Ensure the servo is at 0 degrees on startup
  servo.write(0);
}
void intLCD() {
  // Initialize the LCD with the specified dimensions
  lcd.begin(lcdColumns, lcdRows);
  // Set up the contrast pin as an output
  pinMode(contrastPin, OUTPUT);
  // Display a message
  lcd.print("DOG FEEDER  v1.7");
}
void displayMessage (String message, int column, int line){
     lcd.setCursor(column, line);
      lcd.print(message);
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
      displayMessage ("Serving  portion", 0, 0);
      displayMessage ("of 110g", 4, 1);
      // Delay for 1.5 seconds after object detection (Servo stays open for 1.5 seconds)
      delay(1500);
    
      // Disable ultrasonic sensor after 2 seconds
      ultrasonicTriggered = false; // Reset the ultrasonic trigger flag
      ultrasonicEnabled = false; // Disable ultrasonic sensor
      servo.write(0); // Move servo to 0 degrees
      digitalWrite(whiteLedPin, LOW); // Turn off white LED
      lcd.clear();
      displayMessage ("Take your bowl", 1, 0); // Display "Take your bowl" message
      displayMessage ("Buon appetito!", 1, 1);// Display "Buon appetito!" message
      // Delay for 2.5 seconds before reverting to the initial message
      delay(2500);
      lcd.clear();
      displayMessage ("Place your bowl.", 0, 0);
    } else {
      // If serving and 1.8 seconds have passed, close the servo
      if (ultrasonicTriggered && millis() - servingStartTime >= 1800) {
        ultrasonicTriggered = false; // Reset the ultrasonic trigger flag
        ultrasonicEnabled = false; // Disable ultrasonic sensor
        servo.write(0); // Move servo to 0 degrees
        digitalWrite(whiteLedPin, LOW); // Turn off white LED
        lcd.clear();
        displayMessage ("Place your bowl.", 0, 0);
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
      displayMessage ("WARNING!", 4, 0);
      displayMessage ("Put more food.", 1, 1);
      isAlertDisplayed = true;
    }
    
    // Flicker the red LED when the system is off temporarily
    for (int i = 0; i < 5; i++) {
      digitalWrite(blueLedPin, LOW); // Turn off blue LED when system is down
      delay(300); // Delay for flickering effect
      digitalWrite(redAlertPin, LOW); // Turn off the red LED
      delay(300); // Delay for flickering effect
      digitalWrite(redAlertPin, HIGH); // Turn off the red LED
      delay(300); // Delay for flickering effect
       digitalWrite(redAlertPin, LOW); // Turn off the red LED
      delay(1000); // Delay for flickering effect
    }
  }

}
 
