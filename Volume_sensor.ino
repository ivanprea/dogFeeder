// Define pins for ultrasonic sensor
const int trigPin = 3;
const int echoPin = 2;

// Define pins for LEDs
const int greenLed = A3;
const int yellowLed = A4;
const int redLed = A5;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Initialize LED pins
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
}

void loop() {
  // Variables to store duration and volume
  long duration, volume;
  
  // Clear trigPin before setting high
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set trigPin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read echoPin, pulseIn() returns the duration (in microseconds) it takes for the pulse to return
  duration = pulseIn(echoPin, HIGH);
  
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
  
  // Delay for stability
  delay(100);
}
