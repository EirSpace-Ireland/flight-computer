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

    /// @brief Wake up the sensor and set default ranges (+-2G, +-250deg/s)
    /// @return True if successful otherwise false
    bool init();
    bool reset();

    bool read_accel_axis(IMU_AXIS _axis);
    bool read_accel();
    bool read_gyro_axis(IMU_AXIS _axis);
    bool read_gyro();
    bool read_temp();
    bool read_motion();
    bool read_all();

    bool write_accel_scale(IMU_SCALE _scale);
    bool write_gyro_scale(IMU_SCALE _scale);
    bool write_dlpf_bw();

    int16_t get_acc_x() { return accel[IMU_X]; }
    int16_t get_acc_y() { return accel[IMU_Y]; }
    int16_t get_acc_z() { return accel[IMU_Z]; }
    int16_t get_gyro_x() { return gyro[IMU_X]; }
    int16_t get_gyro_y() { return gyro[IMU_Y]; }
    int16_t get_gyro_z() { return gyro[IMU_Z]; }
    int16_t get_temp() { return temp; };

private:
    bool write_sensor_scale(SENSOR_TYPE _sensor, IMU_SCALE _scale);
    bool read_sensor_axis(SENSOR_TYPE _sensor, IMU_AXIS _axis);
    bool read_sensor(SENSOR_TYPE _sensor);

    bool write(uint8_t _reg, uint8_t* _data, size_t _len);
    bool write_register(uint8_t _reg, uint8_t _data);
    bool read(uint8_t _reg, uint8_t* _data, size_t _len);
    bool read_register_data8(uint8_t _reg, uint8_t* _data);
    bool read_register_data16(uint8_t _reg, uint16_t* _data);
    bool read_register_data32(uint8_t _reg, uint32_t* _data);
};
#endif // EIR_MPU6050
