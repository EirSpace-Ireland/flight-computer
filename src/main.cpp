#include <Arduino.h>
#include <Wire.h>
#include <mpu6050.hpp>

mpu6050 imu;
char buff[50];

unsigned long prevTime = 0;
unsigned long logTime = 0;
int scale = 0;
float conv = 131;
float accel_conv = 16384;
unsigned long imu_timer;

int32_t accel_offset_x;
int32_t accel_offset_y;
int32_t accel_offset_z;

int32_t gyro_offset_x;
int32_t gyro_offset_y;
int32_t gyro_offset_z;

enum FlightState {
  INIT,
  IDLE,
  ACCELERATING,
  COASTING,
  DECENDING,
  LANDED,
  INVALID_STATE
};

FlightState state = INVALID_STATE;

void calibrate_imu(mpu6050* _imu);

void setup()
{
  // Init peripherals
  while(!Serial); // Wait for USB port to be ready
  Serial.begin(115200);
  Wire.setClock(400000);
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);

  // Blink 3 times (1Hz)
  for (size_t i = 0; i < 3; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }

  logTime = prevTime = millis();
  state = INIT;
  imu_timer = 0;
}

void loop(){
  static bool led_state;

  switch (state)
  {
    case INIT:
      Serial.println("Entered INIT state");
      // init and check hardware
        // Sensors are awake and presenst
        // Pyro channels have continuity if configured like that
      // calibrate sensors
      // calibrate_imu()
      // Activate state indcators
      // Test and observe actuators
      state = IDLE;
    break;

    case IDLE:
      Serial.println("Entered IDLE state");
      //Serial.println("Entered IDLE state");
      /**
       * - LEDs and Buzzer
       * - Log slowly
       * - Wait for liftoff
       */

      // log every 100ms
    break;

    case ACCELERATING:
      Serial.println("Entered ACCELERATING state");
      /**
       * - Log quickly
       * - Wait for MEC
       */
    break;

    case COASTING:
      Serial.println("Entered COASTING state");
      /**
       * - Wait for apogee
       *
       */
    break;

    case DECENDING:
      Serial.println("Entered DECENDING state");
      /**
       * - Arm main chute pyro
       * - Look out for main chute deploy
       *
       */
    break;

    case LANDED:
      Serial.println("Entered LANDED state");
      /**
       * - Dump flash to SD
       * - Disarm pyro
       * - Beep loudly
       * - Disable logging
       *
       */
    break;

  default:
    Serial.println("Invalid state!!!");
    break;
  }

}

void calibrate_imu(mpu6050* _imu){
  int32_t accl_accum_x = 0;
  int32_t accl_accum_y = 0;
  int32_t accl_accum_z = 0;

  int32_t gyro_accum_x = 0;
  int32_t gyro_accum_y = 0;
  int32_t gyro_accum_z = 0;

  // accum 1000 values for each axis
  for (size_t i = 0; i < 1000; i++)
  {
    _imu->read_all();

    accl_accum_x += _imu->get_acc_x();
    accl_accum_y += _imu->get_acc_y();
    accl_accum_z += _imu->get_acc_z();

    gyro_accum_x += _imu->get_gyro_x();
    gyro_accum_y += _imu->get_gyro_y();
    gyro_accum_z += _imu->get_gyro_z();
  }

  // get average
  // store in offset valiables
  accel_offset_x = accl_accum_x/1000;
  accel_offset_y = accl_accum_x/1000;
  accel_offset_z = accl_accum_x/1000;

  gyro_offset_x = gyro_accum_x/1000;
  gyro_offset_y = gyro_accum_y/1000;
  gyro_offset_z = gyro_accum_z/1000;
}
