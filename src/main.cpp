#include <Arduino.h>
#include <mpu6050.hpp>
#include <SerialFileLogger.hpp>

enum FlightState
{
  INIT,
  IDLE,
  ACCELERATING,
  COASTING,
  DESCENDING,
  LANDED,
  INVALID_STATE
};

const char *state_string[] = {
    "INIT",
    "IDLE",
    "ACCELERATING",
    "COASTING",
    "DESCENDING",
    "LANDED",
    "INVALID STATE"};

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
SerialFileLogger* logger;

void setup()
{
  // Init peripherals
  logger = Logger<SerialFileLogger>::get_logger();
  Wire.setClock(400000);
  pinMode(LED_BUILTIN, OUTPUT);

  // Blink 3 times
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

void loop()
{

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

    if (!imu.init())
    {
      Serial.println("ERROR: IMU init failed");
      Serial.println("Entering INVALID_STATE");
      next_state = INVALID_STATE;
    }

    Serial.println("IMU init success");
    next_state = IDLE;

    imu.offset_calibration(1000);
    Serial.println("acc_x, acc_y, acc_z, gyr_x, gyr_y, gyr_z, temp");
    break;

  case IDLE:
    // Serial.println("Entered IDLE state");
    /**
     * - LEDs and Buzzer
     * - Log slowly
     * - Wait for liftoff
     */

    // log every 100ms
    if (!imu.read_all())
    {
      Serial.println("ERROR: Failed to read IMU");
      next_state = INVALID_STATE;
    }

    if (millis() - prevLogTime > 200)
    {
      sprintf(buff, "%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f",
              imu.get_acc_x(),
              imu.get_acc_y(),
              imu.get_acc_z(),

              imu.get_gyro_x(),
              imu.get_gyro_y(),
              imu.get_gyro_z(),

              ((imu.get_temp() / 340.0f) + 36.53));
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
