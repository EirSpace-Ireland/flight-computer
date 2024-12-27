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
#include "IMU.hpp"

class mpu6050 : public IMU
{
private:
    int16_t accel_raw[3];
    int16_t gyro_raw[3];
    int16_t temp;
    const uint8_t addr;

    const float accel_scale_factors[4];
    const float gyro_scale_factors[4];

    float accel_conv;
    float gyro_conv;

    enum SENSOR_TYPE
    {
        ACCEL,
        GYRO
    };

public:
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
// interface
    bool init() override;
    bool offset_calibration(const uint16_t num_iterations) override;
    bool read_accel_axis(IMU_AXIS _axis) override;
    bool read_accel() override;
    bool read_gyro_axis(IMU_AXIS _axis) override;
    bool read_gyro() override;
    bool read_all() override;

    float get_acc_x() override { return (accel_raw[IMU_X]-accel_offset[IMU_X]) / accel_conv; };
    float get_acc_y() override { return (accel_raw[IMU_Y]-accel_offset[IMU_Y]) / accel_conv; };
    float get_acc_z() override { return (accel_raw[IMU_Z]-accel_offset[IMU_Z]) / accel_conv; };

    float get_gyro_x() override { return (gyro_raw[IMU_X]-gyro_offset[IMU_X]) / gyro_conv; };
    float get_gyro_y() override { return (gyro_raw[IMU_Y]-gyro_offset[IMU_Y]) / gyro_conv; };
    float get_gyro_z() override { return (gyro_raw[IMU_Z]-gyro_offset[IMU_Z]) / gyro_conv; };

public:
// specific to mpu6050
    mpu6050(bool _addr_0);
    mpu6050();
    ~mpu6050();

    /// @brief Wake up the sensor and set default ranges (+-2G, +-250deg/s)
    /// @return True if successful otherwise false
    bool reset();

    bool read_temp();
    bool read_motion();

    bool write_accel_scale(IMU_SCALE _scale);
    bool write_gyro_scale(IMU_SCALE _scale);
    bool write_dlpf_bw();

    int16_t get_temp() { return temp; };

    int16_t get_acc_x_raw() { return accel_raw[IMU_X]; }
    int16_t get_acc_y_raw() { return accel_raw[IMU_Y]; }
    int16_t get_acc_z_raw() { return accel_raw[IMU_Z]; }

    int16_t get_gyro_x_raw() { return gyro_raw[IMU_X]; }
    int16_t get_gyro_y_raw() { return gyro_raw[IMU_Y]; }
    int16_t get_gyro_z_raw() { return gyro_raw[IMU_Z]; }

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
