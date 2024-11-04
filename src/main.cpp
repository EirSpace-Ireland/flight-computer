#include <Arduino.h>
#include <Wire.h>
#include <mpu6050.hpp>
struct RocetState
{
  int val;
};





RocetState RS;
mpu6050 imu;
char buff[50];

unsigned long prevTime = 0;
unsigned long logTime = 0;
int scale = 0;
float conv = 131;
float accel_conv = 16384;

void setup()
{
  Serial.begin(19200);
  Wire.setClock(400000);
  Wire.begin(); // Initialize comunication
  if (!imu.init())
  {
    Serial.println("IMU INIT FAILED");
    Serial.println("entering while(1) loop");
    while(1);
  }
  Serial.println("IMU INIT SUCCESS");
  logTime = prevTime = millis();
}

void loop()
{
  imu.read_all();

  if (millis() - logTime > 100)
  {
    Serial.print("deg: ");
    Serial.println(imu.get_gyro_x()/conv);

    Serial.print("accel: ");
    Serial.println(imu.get_acc_x()/accel_conv);

    Serial.print("temp: ");
    Serial.println((imu.get_temp()/340.0f) +36.53);

    Serial.println();

    logTime = millis();
  }

  if (millis() - prevTime > 2000)
  {
    Serial.println("Scale update");
    scale++;
    if (scale > 3)
    {
      scale = 0;
    }

    switch (scale)
    {
    case 0:
      imu.write_accel_scale(mpu6050::ACCEL_2G);
      imu.write_gyro_scale(mpu6050::GYRO_250);
      accel_conv = 16384;
      conv = 131;
      break;

    case 1:
      imu.write_accel_scale(mpu6050::ACCEL_4G);
      imu.write_gyro_scale(mpu6050::GYRO_500);
      accel_conv = 8192;
      conv = 65.5;
      break;

    case 2:
      imu.write_accel_scale(mpu6050::ACCEL_8G);
      imu.write_gyro_scale(mpu6050::GYRO_1000);
      accel_conv = 4096;
      conv = 32.8;
      break;

    case 3:
      imu.write_accel_scale(mpu6050::ACCEL_16G);
      imu.write_gyro_scale(mpu6050::GYRO_2000);
      accel_conv = 2048;
      conv = 16.4;
      break;

    default:
      break;
    }
    prevTime = millis();
  }

}
