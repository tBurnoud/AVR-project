//to do : need calibration on start
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050_tockn.h>
//#include <MPU6050.h>
#include <Mouse.h>

//Init MPU
MPU6050 mpu6050(Wire);
int16_t ax, ay, az, gx, gy, gz;
int OX,OY,OZ;

//Button 
const int buttonPin = 7;
int buttonState=0;
int lastState=0;
//Convert from Angle to Distance
int angleToDistance(int a)
{

  if (a < -50){
    return -15;}
  else if (a < -35){
    return -10;
  }
  else if (a < -20){
    return -5;
  }
  else if (a < -17){
    return -1;
 }
  else if (a <= -17){
    return 0;
  }

  
  else if (a > 50){
    return 15;}
  else if (a > 35){
    return 10;
  }
  else if (a > 20){
    return 5;
  }
  else if (a > 17){
    return 1;
  }
  else if (a <=17){
    return 0;
  }
}

void setup() {
// Initialize serial communications at 9600 bps:
Serial.begin(9600);

// Start the mouse interaction
Mouse.begin();

// Start the sensor connection
Wire.begin();
//mpu.initialize();
//if (!mpu.testConnection()) {
//  while (1);
//}
mpu6050.begin();
mpu6050.calcGyroOffsets(true);                              // Setting Gyro offsets
mpu6050.update(); 
OX = mpu6050.getAngleX();                                   // Getting angle X Y Z
OY = mpu6050.getAngleY();
OZ = mpu6050.getAngleZ();

if(OX < 0){                                                 // Inverting the sign of all the three offset values for zero error correction
    OX = OX *(-1);}
else{
    OX = (OX-OX)-OX;}

if(OY < 0){
    OY = (OY *(-1));}
else{
    OY = ((OY-OY)-OY)+10;}

if(OZ < 0){
    OZ = OZ *(-1);}
else{
    OZ = (OZ-OZ)-OZ;}

//  Button Setup
pinMode(buttonPin,INPUT);
}
void loop() {
//  Get Value from sensor
//  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  mpu6050.update();
  ax = OX + mpu6050.getAngleX();                                     // Getting current angle for X Y Z and correcting the zero error
  ay = OY + mpu6050.getAngleY();
//  Z = OZ + mpu6050.getAngleZ();

//  Serial.println(String(ax)+"-"+String(ay));
// Mapping the sensor val
//  int vx = map(ax, -16000, 16000, 90, -90);
//  int vy = map(ay, -16000, 16000, -90, 90);
  Serial.println(String(ax)+"-"+String(ay)+"---"+angleToDistance(ay));
//  Move the cursor
  buttonState=digitalRead(buttonPin);
//  while(buttonState==HIGH){
  Mouse.move(angleToDistance(ay), angleToDistance(ax+10));
//  }

//  Add click action
//  buttonState=digitalRead(buttonPin);

  
  if(lastState==LOW &&buttonState==HIGH){
    click_right();
//    delay(5);
  }   
  lastState=buttonState;

// Delay between read
  delay(20);


}

void click_right(){
  if (!Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.press(MOUSE_LEFT);
      Mouse.release(MOUSE_LEFT);
//      delay(50);
    }
  // else the mouse button is not pressed:
    else {
    // if the mouse is pressed, release it:
      if (Mouse.isPressed(MOUSE_LEFT)) {
        Mouse.release(MOUSE_LEFT);
      }
    }
}
