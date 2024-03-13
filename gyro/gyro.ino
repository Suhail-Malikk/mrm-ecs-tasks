#include <Wire.h>

const int MPU_ADDR = 0x68; 


// variables for gyri data filtering
float prevTime = 0;
float gyroPitch =0;
float gyroRoll = 0;

float firstgyrotime=0;


//furthur variables 
const float alpha = 0.98; // Complementary filter alpha value
const float wait = 5000;
float previous =0;

void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(false);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(28);
  Wire.write(0x10);
  Wire.endTransmission(false);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(27);
  Wire.write(0x10);
  Wire.endTransmission(true);

}
void printdata(float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8)
{
Serial.print(p1);
Serial.print(" ");
Serial.print(p2);
Serial.print(" ");
Serial.print(p3);
Serial.print(" ");
Serial.print(p4);
Serial.print(" ");
Serial.print(p5);
Serial.print(" ");
Serial.print(p6);
Serial.print(" ");
Serial.print(p7);
Serial.print(" ");
Serial.print(p8);
Serial.println(" ");
}
void loop() {
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true); 
  
  float acc_x, acc_y, acc_z;
  float gyro_x, gyro_y, gyro_z;
  
  
  acc_x = (Wire.read() << 8 | Wire.read())/4096.0;
  acc_y = (Wire.read() << 8 | Wire.read())/4096.0;
  acc_z = (Wire.read() << 8 | Wire.read())/4096.0;
  
  
  Wire.read();
  Wire.read();
  
  
  gyro_x = (Wire.read() << 8 | Wire.read())/32.8; //pitch
  gyro_y = (Wire.read() << 8 | Wire.read())/32.8; //roll
  gyro_z = (Wire.read() << 8 | Wire.read())/32.8; //yaw
  float currentTime = millis();
  
  #define PI 3.14159
  float acc_pitch, acc_roll;
  acc_pitch = (atan2(acc_y, sqrt(acc_x*acc_x+acc_z*acc_z )))*180/PI;

  acc_roll = (atan2(acc_x, sqrt(acc_y*acc_y+acc_z*acc_z )))*180/PI;

  float gyrotime=millis();
  float gyro_roll=0;
  gyro_roll = gyro_roll + (gyrotime- firstgyrotime)*gyro_x*0.001;
  //gyro_roll = gyro_roll + gyro_x / 32.8; // Gyroscope scaling factor for 250 deg/s range
  float gyro_pitch=0;
  gyro_pitch = gyro_pitch + (gyrotime- firstgyrotime)*gyro_y*0.001;
  //gyro_pitch =gyro_pitch+  gyro_y / 32.8;
  firstgyrotime = gyrotime;
  
  float roll = alpha * gyro_roll + (1 - alpha) * acc_roll;
  float pitch = alpha * gyro_pitch + (1 - alpha) * acc_pitch;
  
  //delay(1000); 

  //float timediff;
  //timediff = currentTime-prevTime;
  //Serial.print("time delay diff: ");
  //Serial.println(timediff);

  //gyroPitch = gyroPitch + gyro_x*(currentTime - prevTime)*0.001;
  //gyroRoll = gyroRoll + gyro_y*(currentTime - prevTime)*0.001;
  //prevTime = currentTime;

  
  
  // Serial.print(gyroPitch);
  // Serial.print(" ");
  // Serial.println(gyroRoll);
  // Serial.println(" ");
  

  float timenow= millis();
  float const gyro_x_const = gyroPitch;
  float const gyro_y_const;
  float slope;

  if (timenow-previous > wait){

    float filterGyro_x_new = gyroPitch;
    
    slope = atan2(filterGyro_x_new - gyro_x_const, wait);

  }

  float gyro_x_new = gyroPitch - slope;

  printdata(acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, acc_pitch, acc_roll);
  //Serial.println();
  //Serial.print("final gyro pitch: ");
  //Serial.println(gyro_x_new);
  
  //Serial.println();
  //Serial.println();

   
  
}