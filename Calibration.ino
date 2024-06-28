#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define CENTREPOS 1500
#define MINPOS 500
#define MAXPOS 2500

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

uint8_t servonum = 0; // address of servo to be calibrated
String readString;
int pos = CENTREPOS;

void setup() {
  Serial.begin(9600);
  pos=CENTREPOS; // Initial position
  pwm.begin();
  pwm.setOscillatorFrequency(25000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  Serial.println("Servo calibration");
  Serial.println("Use this to calibrate your servo to find the range of movement required");
  Serial.println("The servo should start close to the centre of the range");
  Serial.println("To select the servo to calibrate, type the servo number followed by 's': e.g. 2s or 15s");
  Serial.println("Type a value followed by a + to move in one direction or a valu followed by a - to move in the other direction");
  Serial.println("For example 100+ to 200-");  
  Serial.println("To move to a specific location use strings like 900x or 1800x for new servo position");
  Serial.println("Move the servo to find the required range for whatever you're operating.");
  Serial.println("Servos min and max can vary, try the 1000 - 2000 range to start with.");
  Serial.println("WARNING: Exceeding the max range could damage the servo.");
  Serial.print("NOTE: To try to protect the servo: this sketch limits the minimum position to: ");
  Serial.print(MINPOS);
  Serial.print(", and the maximum position to: ");
  Serial.println(MAXPOS);
  Serial.println();
  pwm.writeMicroseconds(servonum, pos);
  Serial.print("Servo: ");
  Serial.println(servonum);
  Serial.print("Centre point: ");  
  Serial.println(pos);  
  delay(10);
}

void loop()
{
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }
  if (readString.length() >0) {
    int oldPos = pos;

    if(readString.indexOf('x') >0) {
      pos = readString.toInt();
    }
    else if(readString.indexOf('+') >0) {
      pos = pos + readString.toInt();
    }
    else if(readString.indexOf('-') >0) {
      pos = pos - readString.toInt();
    }
    else if (readString.indexOf('s') > 0)
    {
      servonum = readString.toInt();
      pos = CENTREPOS;
    }

    if (pos < MINPOS || pos > MAXPOS)
    {
      // invalid position
      pos = oldPos;
    }

    pwm.writeMicroseconds(servonum, pos);
    Serial.print("Servo: ");
    Serial.print(servonum); 
    if (pos == CENTREPOS)
    {
      Serial.print(" Centre Point: ");
    }
    else
    {
      Serial.print(" Position: ");
    }
    Serial.println(pos);
    readString=""; //empty for next input
  }
}
