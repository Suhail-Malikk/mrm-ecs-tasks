#include <Wire.h>

#define MPU 0x68
#define acc_con 0x1C
#define gyro_con 0x1B
#define acc_XOUT_H 0x3B
#define gyro_XOUT_H 0x43

int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;


float kalman_roll= 0, uncertainroll= 2*2;
float kalman_pitch =0, uncertainpitch= 2*2; 

float pitch;
float roll;

float gyro_pitch;
float gyro_roll;

float Output[]={0,0};


void setup() {
  Wire.begin();
  Serial.begin(9600);
  initializeMPU9255();
}


void initializeMPU9255() {
  Wire.beginTransmission(MPU);
  Wire.write(acc_con);
  Wire.write(0x01);
  Wire.endTransmission();

  Wire.beginTransmission(MPU);
  Wire.write(gyro_con);
  Wire.write(0x01);
  Wire.endTransmission();
}



void readData() {
  Wire.beginTransmission(MPU);
  Wire.write(acc_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  accelerometer_x = (Wire.read() << 8 | Wire.read())/4096;
  accelerometer_y = (Wire.read() << 8 | Wire.read())/4096;
  accelerometer_z = (Wire.read() << 8 | Wire.read())/4096.0;

  Wire.beginTransmission(MPU);
  Wire.write(gyro_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  gyro_x = (int16_t)(Wire.read() << 8 | Wire.read()) / 65.5;
  gyro_y = (int16_t)(Wire.read() << 8 | Wire.read()) / 65.5;
  gyro_z = (int16_t)(Wire.read() << 8 | Wire.read()) / 65.5; 
}



void loop() {
  readData();
    pitch = atan2(accelerometer_y, sqrt(accelerometer_x * accelerometer_x + accelerometer_z * accelerometer_z) )*180/3.14;
   roll = atan2(accelerometer_x, sqrt(accelerometer_y * accelerometer_y + accelerometer_z * accelerometer_z) )*180/3.14;

   gyro_pitch = pitch + (gyro_x) * 0.01; 
   gyro_roll = roll + (gyro_y) * 0.01;  

  Serial.print(pitch);
  Serial.print(" ");
  Serial.print(roll);
  Serial.print(" ");

  kalmanFilter(kalman_roll, uncertainroll,gyro_roll , roll);
  kalman_roll= Output[0];
  uncertainroll= Output[1];

  kalmanFilter(kalman_pitch, uncertainpitch, gyro_pitch, pitch);
  kalman_pitch= Output[0];
  uncertainpitch= Output[1];
  
  Serial.print(kalman_roll);
  Serial.print(" ");
  Serial.println(kalman_pitch);
  Serial.print(" ");
}

void kalmanFilter(float state, float uncertainity, float input, float measurement)
{
  const float processNoise = 0.003;
  const float measurementNoise = 0.3;
  
  state = state + processNoise*input;
  uncertainity= uncertainity + processNoise*processNoise*4*4;
  float kalmangain = uncertainity*1/(1*uncertainity + measurementNoise*measurementNoise);
  state= state+ kalmangain*(measurement-state);
  uncertainity=(1-kalmangain)*uncertainity;

  Output[0]=state;
  Output[1]=uncertainity;  

}
