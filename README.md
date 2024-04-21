# Dog Feeder Wiring

## Tilt Sensor (Tilt Switch):
- One leg to ground (GND).
- The other leg to digital pin 2 (tiltSensorPin).

## LEDs:
- **Green:**
Anode (long leg) to digital pin 3.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).

 - **Red:**
Anode (long leg) to digital pin 4.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).

 - **White:**
Anode (long leg) to analog pin A1.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).

 - **Blue:**
Anode (long leg) to digital pin A2.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).

 - **Red ALERT:**
Anode (long leg)to digital pin A3.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).

## Ultrasonic Sensor (HC-SR04):
- VCC pin of the ultrasonic sensor to 5V.
- GND pin of the ultrasonic sensor to ground (GND).
- TRIG pin of the ultrasonic sensor to digital pin 5 (ultrasonicTrigPin).
- ECHO pin of the ultrasonic sensor to digital pin 6 (ultrasonicEchoPin).

## Servo Motor:
 - VCC (red) to 5V.
 - GND (brown) to ground (GND).
 - Connect the signal (orange) to digital pin 7 (servoPin).

## LCD (16x2):
 - VSS (ground) pin of the LCD to ground (GND).
 - VDD (power) pin of the LCD to 5V.
 - VO (contrast) pin of the LCD to the wiper (middle pin) of a potentiometer.
 - Connect one outer pin of the potentiometer to 5V and the other outer pin to ground (GND).
 - RS pin of the LCD to digital pin 8 (rs).
 - E pin of the LCD to digital pin 9 (en).
 - D4, D5, D6, and D7 pins to digital pins 10, 11, 12, and 13 respectively (d4, d5, d6, d7).
 - Contrast Potentiometer:
 - Connect one outer pin of the potentiometer to 5V and the other outer pin to ground (GND).

![image](https://github.com/ivanprea/dogFeeder/assets/78477048/d39db798-0cf4-4000-bd24-05784a1b07d1)

