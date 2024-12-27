#ifndef EIR_MPU6050
#define EIR_MPU6050

/**
 * @file mpu6050.hpp
 * @author Ugochukwu Uzoukwu (nozolo96@gmail.com)
 * @brief Driver for the MPU6050 IMU sensor
 * @version 0.1
 * @date 2024-12-27
 *
 * @copyright Copyright (c) 2024 EirSpace Rocketry
 *
 */

#include <Arduino.h>
#include <Wire.h>

/**
 * @brief MPU6050 IMU sensor driver
 *
 */
class mpu6050
{
private:
    int16_t accel[3];
    int16_t gyro[3];
    int16_t temp;
    int32_t accel_calib[3];
    int32_t gyro_calib[3];
    const uint8_t addr;

    enum SENSOR_TYPE
    {
        ACCEL,
        GYRO
    };

public:
    enum IMU_AXIS : uint8_t
    {
        IMU_X = 0,
        IMU_Y,
        IMU_Z
    };

    enum IMU_SCALE : uint8_t
    {
        ACCEL_2G = 0,
        ACCEL_4G,
        ACCEL_8G,
        ACCEL_16G,
        GYRO_250 = 0,
        GYRO_500,
        GYRO_1000,
        GYRO_2000
    };

    enum DLPF_BW : uint8_t
    {
        BW_260,
        BW_184,
        BW_94,
        BW_44,
        BW_21,
        BW_10,
        BW_5
    };

public:
    mpu6050(bool _addr_0);
    mpu6050();
    ~mpu6050();

    /**
     * @brief Verify connection to MPU6050 and wake from sleep mode
     *
     * @return True on success, false on failure
     */
    bool init();

    /**
     * @brief Reset the MPU6050
     *
     * @return True on success, false on failure
     */
    bool reset();

    /**
     * @brief Read a specific accelerometer axis
     *
     * @param _axis The axis to read
     * @return True on success, false on failure
     */
    bool read_accel_axis(IMU_AXIS _axis);

    /**
     * @brief Read all accelerometer axes
     *
     * @return True on success, false on failure
     */
    bool read_accel();

    /**
     * @brief Read a specific gyroscope axis
     *
     * @param _axis The axis to read
     * @return True on success, false on failure
     */
    bool read_gyro_axis(IMU_AXIS _axis);

    /**
     * @brief Read all gyroscope axes
     *
     * @return True on success, false on failure
     */
    bool read_gyro();

    /**
     * @brief Read temperature
     *
     * @return True on success, false on failure
     */
    bool read_temp();

    /**
     * @brief Read all motion data
     *
     * @return True on success, false on failure
     */
    bool read_motion();

    /**
     * @brief Read all sensor data
     *
     * @return True on success, false on failure
     */
    bool read_all();

    /**
     * @brief Set the full scale range for the accelerometer
     *
     * @return True on success, false on failure
     */
    bool write_accel_scale(IMU_SCALE _scale);

    /**
     * @brief Set the full scale range for the gyroscope
     *
     * @return True on success, false on failure
     */
    bool write_gyro_scale(IMU_SCALE _scale);

    /**
     * @brief Set the digital low pass filter bandwidth
     *
     * @return True on success, false on failure
     */
    bool write_dlpf_bw();

    /**
     * @brief Get the raw accelerometer x-axis value
     *
     * @return 16-bit signed integer representing acceleration in Gs
     */
    int16_t get_acc_x() { return accel[IMU_X]; }

    /**
     * @brief Get the raw accelerometer y-axis value
     *
     * @return 16-bit signed integer representing acceleration in Gs
     */
    int16_t get_acc_y() { return accel[IMU_Y]; }

    /**
     * @brief Get the raw accelerometer z-axis value
     *
     * @return 16-bit signed integer representing acceleration in Gs
     */
    int16_t get_acc_z() { return accel[IMU_Z]; }

    /**
     * @brief Get the raw gyroscope x-axis value
     *
     * @return 16-bit signed integer representing angular velocity in degrees per second
     */
    int16_t get_gyro_x() { return gyro[IMU_X]; }

    /**
     * @brief Get the raw gyroscope y-axis value
     *
     * @return 16-bit signed integer representing angular velocity in degrees per second
     */
    int16_t get_gyro_y() { return gyro[IMU_Y]; }

    /**
     * @brief Get the raw gyroscope z-axis value
     *
     * @return 16-bit signed integer representing angular velocity in degrees per second
     */
    int16_t get_gyro_z() { return gyro[IMU_Z]; }

    /**
     * @brief Get the raw temperature value
     *
     * @return 16-bit signed integer representing temperature in degrees Celsius
     */
    int16_t get_temp() { return temp; };

private:
    /**
     * @brief Helper function to configure sensor full scale range
     *
     * @param _sensor The sensor to configure
     * @return True on success, false on failure
     */
    bool write_sensor_scale(SENSOR_TYPE _sensor, IMU_SCALE _scale);

    /**
     * @brief Helper function to read a specific sensor axis
     *
     * @param _sensor The sensor to read
     * @param _axis The axis to read
     * @return True on success, false on failure
     */
    bool read_sensor_axis(SENSOR_TYPE _sensor, IMU_AXIS _axis);

    /**
     * @brief Helper function to read all sensor axes
     *
     * @param _sensor The sensor to read
     * @return True on success, false on failure
     */
    bool read_sensor(SENSOR_TYPE _sensor);

    /**
     * @brief Helper function to write multiple bytes to range of registers
     *
     * @param _reg The base register to start writing from
     * @param _data The data to write
     * @param _len The length of the data
     * @return True on success, false on failure
     */
    bool write(uint8_t _reg, uint8_t* _data, size_t _len);

    /**
     * @brief Helper function to write a single byte to a register
     *
     * @param _reg The register to write to
     * @param _data The data to write
     * @return True on success, false on failure
     */
    bool write_register(uint8_t _reg, uint8_t _data);

    /**
     * @brief Helper function to read multiple bytes from range of registers
     *
     * @param _reg The base register to start reading from
     * @param _data The buffer to store the data
     * @param _len The length of the data
     * @return True on success, false on failure
     */
    bool read(uint8_t _reg, uint8_t* _data, size_t _len);

    /**
     * @brief Helper function to read a single byte from a register
     *
     * @param _reg The register to read from
     * @param _data The variable to store the data
     * @return True on success, false on failure
     */
    bool read_register_data8(uint8_t _reg, uint8_t* _data);

    /**
     * @brief Helper function to read a 16-bit value from a register
     *
     * @param _reg The register to read from
     * @param _data The variable to store the data
     * @return True on success, false on failure
     */
    bool read_register_data16(uint8_t _reg, uint16_t* _data);

    /**
     * @brief Helper function to read a 32-bit value from a register
     *
     * @param _reg The register to read from
     * @param _data The variable to store the data
     * @return True on success, false on failure
     */
    bool read_register_data32(uint8_t _reg, uint32_t* _data);
};
#endif // EIR_MPU6050
