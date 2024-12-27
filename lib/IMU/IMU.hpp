#ifndef EIR_IMU
#define EIR_IMU

/**
 * @file IMU.hpp
 * @author Ugochukwu Uzoukwu (nozolo96@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-12-27
 *
 * @copyright Copyright (c) 2024 EirSpace Rocketry
 *
 */

#include <Arduino.h>

/**
 * @brief Abstract class for IMU sensors
 *
 */
class IMU
{
protected:
    float accel[3];
    float gyro[3];
    int16_t accel_offset[3];
    int16_t gyro_offset[3];

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

    IMU()
    {}
    virtual ~IMU() {};

    virtual bool init();
    virtual bool offset_calibration(const uint16_t num_iterations);

    virtual bool read_accel_axis(IMU_AXIS _axis);
    virtual bool read_accel();
    virtual bool read_gyro_axis(IMU_AXIS _axis);
    virtual bool read_gyro();
    virtual bool read_all();

    virtual float get_acc_x();
    virtual float get_acc_y();
    virtual float get_acc_z();
    virtual float get_gyro_x();
    virtual float get_gyro_y();
    virtual float get_gyro_z();
};

#endif // EIR_IMU
