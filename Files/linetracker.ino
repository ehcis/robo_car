//CSE 5410: FINAL ROBOT CAR PROJECT

#include <Wire.h> //Library needed to communicate with the I2C
#include "Adafruit_TCS34725.h"  //Needed to control the line sensor

Adafruit_TCS34725 tcs = Adafruit_TCS34725();  //Initializes the sensor with the default values 
#define CW 0
#define CCW 1
#define MOTOR_A 0
#define MOTOR_B 1

const byte PWMA = 3; // PWM control (speed) for motor A
const byte PWMB = 11; // PWM control (speed) for motor B
const byte DIRA = 2; // Direction control for motor A
const byte DIRB = 4; // Direction control for motor B

//Line Sensors: 
int qreLeft = A0; //connected to Analog 0
int qreRight = A2; //connected to Analog 2

//Robot Car Speeds:
byte spd = 170; // forward speed
byte hiSpd = 170; //turning speed

//Needed for the line sensors to stay on track 
int threshold = 750;

void setupArdumoto()
{
  // All pins should be set up as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  
  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}

void setup()
{
  setupArdumoto(); // Function to initialize all motor drive pins
  
  if(tcs.begin())
  {
  }
  else
  {
    allStop();
    while (1);
  }
}

void loop()
{
  uint16_t r, g, b, c, colorTemo, lux;
  
  int QRE_Left = analogRead(qreLeft); // Analog output from left sensor
  int QRE_Right = analogRead(qreRight); // Analog output from right sensor
  tcs.getRawData(&r, &g, &b, &c);

  if (r>50) 
  {
    if (QRE_Left > threshold && QRE_Right > threshold)
    {
      forward();
    }
    else if (QRE_Left < threshold && QRE_Right > threshold) // if left sensor is off-track,turn right
    {
      bearRight();
    }
    else if (QRE_Left > threshold && QRE_Right < threshold) // If right sensor is off-track, turn left
    {
      bearLeft();
    }
  }
  else
  {
    allStop();
    delay(2000);
    forward();
    delay(100);
  }
}

void driveArdumoto(byte motor, byte dir, byte spd)  //Runs the motors in a specified direction and speed. 
 {
  if (motor == MOTOR_A)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
   }
  else if (motor == MOTOR_B)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }
}

void forward() // Runs both motors at the given speed 
{
  driveArdumoto(MOTOR_A, CW, spd); // Motor A at speed spd
  driveArdumoto(MOTOR_B, CW, spd); // Motor B at speed spd
}

void bearRight()
{
  driveArdumoto(MOTOR_B, CW, 0); //Motor B STOP
  driveArdumoto(MOTOR_A, CW, hiSpd); //Motor A hiSpd
}

void bearLeft()
{
  driveArdumoto(MOTOR_B, CW, hiSpd); //Motor B hiSpd
  driveArdumoto(MOTOR_A, CW, 0); //Motor A STOP
}

void stopArdumoto(byte motor) //Makes the motors stop by setting them to 0. 
{
  driveArdumoto(motor, 0, 0);
}

void allStop() //This function stops both the motors
{
  stopArdumoto(MOTOR_A);

  // All pins should be set up as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  
  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
