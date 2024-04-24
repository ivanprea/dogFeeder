#include <Servo.h>
#include <LiquidCrystal.h>

// LEDs Pins 
// LEDs obj detection
const int whiteLedPin = A0;         // White LED (ON when sensor in pin 5 and 6 detect obj)
const int blueLedPin = A1;          // Blue LED (Alaways oN)
const int redAlertPin = A2;         // Red LED (Flicks when yellow LED is OFF)
// LEDs food detection
const int greenLed = A3;            // Green LED (ON when sensor in pin 3 and 4 detect food at minum distance)
const int yellowLed = A4;           // Yellow LED (ON when sensor in pin 3 and 4 detect food at medium distance)
const int redLed = A5;              // Red LED (ON when sensor in pin 3 and 4 detect food at max distance)
const int contrastPin = 1;          // Potentiometer connected to adjust contrast

// Ultrasonic sensors Pins 
// Pins Bowl Ultrasonic sensor
const int foodTrigPin = 5;         // Ultrasonic sensor TRIG pin
const int foodEchoPin = 6;         // Ultrasonic sensor ECHO pin
// Pins Food Ultrasonic sensor
const int objTrigPin = 4;
const int objEchoPin = 3;

// Servo motor Pins 
const int servoPin = 7;            // Servo motor signal pin

// LCD 
// LCD Pins 
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
  initLcd();
  initLeds();
  initServo();
  initUsensor();
}
void initLeds() {
  pinMode(greenLed, OUTPUT); // Initialize LED pins
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(whiteLedPin, OUTPUT); 
  pinMode(blueLedPin, OUTPUT);
  pinMode(redAlertPin, OUTPUT);  
  digitalWrite(blueLedPin, HIGH);   // Turn on blue LED
}
void initServo() {
  servo.attach(servoPin);
  pinMode(servoPin, OUTPUT);  // Initialize servo pin
  servo.write(0);  // Ensure the servo is at 0 degrees on startup
}
void initLcd() {
  lcd.begin(lcdColumns, lcdRows);   // Initialize the LCD with the specified dimensions
  pinMode(contrastPin, OUTPUT);    // Set up the contrast pin as an output
  lcd.print("DOG FEEDER  v1.8");  // Display a message on startup
}
void initUsensor() {
  pinMode(foodTrigPin, OUTPUT);   // Initialize Food sensor
  pinMode(foodEchoPin, INPUT);
  pinMode(objTrigPin, OUTPUT);      // Initialize Obj sensor
  pinMode(objEchoPin, INPUT);
}
void displayMessage(String message, int column, int line) {
  lcd.setCursor(column, line);
  lcd.print(message);
}

void loop() {
  // Read ultrasonic sensor distance
  long duration, distance;
  long volume;
   // Turn on blue LED
  digitalWrite(blueLedPin, HIGH);

  digitalWrite(objTrigPin, LOW);
  delayMicroseconds(2);

  // Set trigPin high for 10 microseconds
  digitalWrite(objTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(objTrigPin, LOW);

  // Read echoPin, pulseIn() returns the duration (in microseconds) it takes for the pulse to return
  duration = pulseIn(objEchoPin, HIGH);

  // Calculate volume in cm³
  volume = duration * 0.034 / 2;

  // Output volume to serial monitor
  Serial.print("Volume: ");
  Serial.println(volume);

  // Control LEDs based on volume
  if (volume >= 0 && volume <= 15) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(redLed, HIGH);
  } else if (volume > 15 && volume <= 30) {
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, HIGH);
    digitalWrite(redLed, HIGH);
  } else if (volume > 30 && volume <= 50) {
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, HIGH);
  } else {
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, LOW);
  }

  // Enable or disable ultrasonic sensor based on ultrasonicEnabled flag
  if (ultrasonicEnabled) {
    digitalWrite(foodTrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(foodTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(foodTrigPin, LOW);
    duration = pulseIn(foodEchoPin, HIGH);
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
    ultrasonicEnabled = true;             // Enable ultrasonic sensor
    servingStartTime = millis();          // Record the time when serving started
  }

  // If ultrasonic is triggered and object detected within 10 cm, move servo to 90 degrees
  if (ultrasonicTriggered && distance < 10) {
    servo.write(90);               // Move servo to 90 degrees
    digitalWrite(whiteLedPin, HIGH); // Turn on white LED
    lcd.clear();
    displayMessage("Serving  portion", 0, 0);   
    displayMessage("of 110g", 4, 1); 

    // Delay for 1.5 seconds after object detection (Servo stays open for 1.5 seconds)
    delay(1500);

    // Disable ultrasonic sensor after 2 seconds
    ultrasonicTriggered = false; // Reset the ultrasonic trigger flag
    ultrasonicEnabled = false;   // Disable ultrasonic sensor
    servo.write(0);              // Move servo to 0 degrees
    digitalWrite(whiteLedPin, LOW); // Turn off white LED
    lcd.clear();
    displayMessage("Take your bowl", 1, 0);   
    displayMessage("Buon appetito!", 1, 1); 
    // Delay for 2.5 seconds before reverting to the initial message
    delay(2500);
    lcd.clear();
    displayMessage("Place your bowl.", 0, 0);
  } else {
    // If serving and 1.8 seconds have passed, close the servo
    if (ultrasonicTriggered && millis() - servingStartTime >= 1800) {
      ultrasonicTriggered = false; // Reset the ultrasonic trigger flag
      ultrasonicEnabled = false;   // Disable ultrasonic sensor
      servo.write(0);              // Move servo to 0 degrees
      digitalWrite(whiteLedPin, LOW); // Turn off white LED
      lcd.clear();
      displayMessage("Place your bowl.", 0, 0);
    }
  }

  // If ultrasonic has been triggered and 3000 ms have passed since the last trigger, enable ultrasonic sensor
  if (ultrasonicTriggered && millis() - ultrasonicLastTriggerTime >= 3000) {
    ultrasonicEnabled = true; // Enable ultrasonic sensor
  }

  // Flicker red alert LED when yellow LED goes off
  if (digitalRead(yellowLed) == LOW) {
    servo.write(0);
    lcd.clear();
    displayMessage("WARNING", 4, 0);
    displayMessage("Put more food.", 1, 1);
     
    // Turn on red alert LED for 500ms
    digitalWrite(redAlertPin, HIGH);
    delay(500);
    // Turn off red alert LED for 500ms
    digitalWrite(redAlertPin, LOW);
    delay(500);
    // Turn on red alert LED for 500ms
    digitalWrite(redAlertPin, HIGH);
    delay(500);
    // Turn off red alert LED for 1000ms
    digitalWrite(redAlertPin, LOW);
    delay(1000);
    // Turn off evething except for red LED, red alert LED, Ultrasonic sensor in pins 2 and 3, and LCD displaying the warning message
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
    
    digitalWrite(whiteLedPin, LOW);
    ultrasonicEnabled = false;
    servo.write(0);
  } else {
    // Re-enable servo motor once error message goes away
    if (!ultrasonicEnabled && !ultrasonicTriggered) {
      servo.attach(servoPin);
    }
  }

  // Delay for a short time to prevent flickering
  delay(100); // Adjust delay as needed
}



