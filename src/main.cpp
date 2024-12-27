#include <Arduino.h>
#include <mpu6050.hpp>

enum FlightState {
  INIT,
  IDLE,
  ACCELERATING,
  COASTING,
  DESCENDING,
  LANDED,
  INVALID_STATE
};

const char* state_string[] = {
  "INIT",
  "IDLE",
  "ACCELERATING",
  "COASTING",
  "DESCENDING",
  "LANDED",
  "INVALID STATE"
};

// Global variables
FlightState state = INVALID_STATE;
FlightState next_state = INVALID_STATE;

char buff[1024];

unsigned long prevTime = 0;
unsigned long prevLogTime = 0;
int scale = 0;
float gyro_conv = 131;
float accel_conv = 16384;
unsigned long imu_timer;

int32_t accel_offset_x;
int32_t accel_offset_y;
int32_t accel_offset_z;

int32_t gyro_offset_x;
int32_t gyro_offset_y;
int32_t gyro_offset_z;

// Objects
mpu6050 imu;

// Function prototypes
void calibrate_imu(mpu6050* _imu);

void setup()
{
  // Init peripherals
  while(!Serial); // Wait for USB port to be ready
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000);
  pinMode(LED_BUILTIN, OUTPUT);

  // Blink 3 times (1Hz)
  for (size_t i = 0; i < 3; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }

  prevLogTime = millis();
  next_state = INIT;
  imu_timer = 0;
}

void loop(){

  if (state != next_state)
  {
    state = next_state;
    Serial.print("INFO: Entered state [");
    Serial.print(state_string[state]);
    Serial.println("]");
  }

  switch (state)
  {
    case INIT:
      /**
       * - Init and check hardware
       * - Calibrate sensors
       * - Activate state indicators
       * - Test and observe actuators
       */
      Serial.println("DEBUG: Run imu.init()");

      if (imu.init() == false)
      {
        Serial.println("ERROR: IMU init failed");
        Serial.println("Entering INVALID_STATE");
        next_state = INVALID_STATE;
      }
      else
      {
        Serial.println("IMU init success");
        next_state = IDLE;
      }

      calibrate_imu(&imu);
      Serial.println("acc_x, acc_y, acc_z, gyr_x, gyr_y, gyr_z, temp");
    break;

    case IDLE:
      //Serial.println("Entered IDLE state");
      /**
       * - LEDs and Buzzer
       * - Log slowly
       * - Wait for liftoff
       */

      // log every 100ms
      imu.read_all();
      if (imu.read_all() == false)
      {
        Serial.println("ERROR: Failed to read IMU");
        next_state = INVALID_STATE;
      }

      if (millis() - prevLogTime > 200)
      {
        sprintf(buff, "%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f",
          ((imu.get_acc_x()-accel_offset_x)/accel_conv),
          ((imu.get_acc_y()-accel_offset_y)/accel_conv),
          ((imu.get_acc_z()-accel_offset_z)/accel_conv),

          ((imu.get_gyro_x()-gyro_offset_x)/gyro_conv),
          ((imu.get_gyro_y()-gyro_offset_y)/gyro_conv),
          ((imu.get_gyro_z()-gyro_offset_z)/gyro_conv),

          ((imu.get_temp()/340.0f) + 36.53)
        );
        Serial.println(buff);

        prevLogTime = millis();
      }
    break;

    case ACCELERATING:
      /**
       * - Log quickly
       * - Wait for MEC
       */
    break;

    case COASTING:
      /**
       * - Wait for apogee
       *
       */
    break;

    case DESCENDING:
      /**
       * - Arm main chute pyro
       * - Look out for main chute deploy
       *
       */
    break;

    case LANDED:
      /**
       * - Dump flash to SD
       * - Disarm pyro
       * - Beep loudly
       * - Disable logging
       *
       */
    break;

  case INVALID_STATE:
    break;
  }
}

void calibrate_imu(mpu6050* _imu){
  const int32_t num_iterations = 1000;
  int32_t accl_accum_x = 0;
  int32_t accl_accum_y = 0;
  int32_t accl_accum_z = 0;

  int32_t gyro_accum_x = 0;
  int32_t gyro_accum_y = 0;
  int32_t gyro_accum_z = 0;

  // accum num_iterations values for each axis
  for (size_t i = 0; i < num_iterations; i++)
  {
    _imu->read_all();

    accl_accum_x += _imu->get_acc_x();
    accl_accum_y += _imu->get_acc_y();
    accl_accum_z += _imu->get_acc_z();

    gyro_accum_x += _imu->get_gyro_x();
    gyro_accum_y += _imu->get_gyro_y();
    gyro_accum_z += _imu->get_gyro_z();

    if (i%200 == 0)
    {
      Serial.println("*");
      Serial.println(_imu->get_acc_x());
      Serial.println(_imu->get_acc_y());
      Serial.println(_imu->get_acc_z());
      Serial.println();
      Serial.println(_imu->get_gyro_x());
      Serial.println(_imu->get_gyro_y());
      Serial.println(_imu->get_gyro_z());
    }
  }
  Serial.print("\n");

  // get average
  // store in offset valiables
  accel_offset_x = accl_accum_x/num_iterations;
  accel_offset_y = accl_accum_y/num_iterations;
  accel_offset_z = (accl_accum_z/num_iterations) - (INT16_MAX/2);


  gyro_offset_x = gyro_accum_x/num_iterations;
  gyro_offset_y = gyro_accum_y/num_iterations;
  gyro_offset_z = gyro_accum_z/num_iterations;

  Serial.println("AVG OFFSETS");
  Serial.println("+=====+");
  Serial.println("ACCEL");
  Serial.println(accel_offset_x);
  Serial.println(accel_offset_y);
  Serial.println(accel_offset_z);
  Serial.println();

  Serial.println("GYRO");
  Serial.println(gyro_offset_x);
  Serial.println(gyro_offset_y);
  Serial.println(gyro_offset_z);
  Serial.println();
  delay(5000);
}
