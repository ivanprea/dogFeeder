## This is the schematic and the code for hardware designed to control a dog/cat feeder

# How does it work

## 1. Initialization: 
The setup function is responsible for initializing the LCD, LEDs, servo motor, and ultrasonic sensors.
## 2. Ultrasonic Sensor Readings:
Food Detector (Pins 3 and 4): This component monitors the level of food in the container. It controls the Red, Yellow, and Green LEDs according to the detected level.
Bowl Detector (Pins 5 and 6): This detects the presence of a dog bowl. It triggers the servo motor to open when the bowl is detected.
## 3. Servo Motor Control:
When the dog bowl is detected within a certain distance by the Bowl Detector, the servo motor is engaged to dispense food. It moves from 0 to 90 degrees, releasing food into the bowl. 
After a predefined period (1.5 seconds), it returns to its initial position (0 degrees). Meanwhile, a message appears on the LCD indicating that food is being served.
## 4. Alerts:
If the level of food in the container, as determined by the Food Detector, is low (signaled by the red LED being the only one illuminated), an alert is triggered. 
This alert is displayed on the LCD, while the red alert LED flashes to indicate the need for additional food. Other LEDs are deactivated, and both the servo motor and Bowl Detector are disabled.
## 5. Recovery: 
Once the low food condition is rectified (e.g., more food is added to the container), the system resumes normal operation.
