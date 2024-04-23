# Dog Feeder Wiring


## LEDs:

 - **White:**
Anode (long leg) to analog pin A0.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).

 - **Blue:**
Anode (long leg) to digital pin A1.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).

 - **Red ALERT:**
Anode (long leg)to digital pin A2.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).
- **Green:**
Anode (long leg) to digital pin A3.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).

- **Yellow:**
Anode (long leg)to digital pin A4.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).

 - **Red:**
Anode (long leg) to digital pin A5.
Cathode (short leg) to a current-limiting resistor (usually 220 ohms), then connect the other end of the resistor to ground (GND).


## Ultrasonic Sensor (HC-SR04) Bowl detector:
- VCC pin of the ultrasonic sensor to 5V.
- GND pin of the ultrasonic sensor to ground (GND).
- TRIG pin of the ultrasonic sensor to digital pin 5.
- ECHO pin of the ultrasonic sensor to digital pin 6.

- ## Ultrasonic Sensor (HC-SR04) Food detector:
- VCC pin of the ultrasonic sensor to 5V.
- GND pin of the ultrasonic sensor to ground (GND).
- TRIG pin of the ultrasonic sensor to digital pin 4.
- ECHO pin of the ultrasonic sensor to digital pin 3.

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



![Schematic V1 8_bb](https://github.com/ivanprea/dogFeeder/assets/78477048/1ba52478-f6a7-4ee3-aa41-5400d878e00f)


