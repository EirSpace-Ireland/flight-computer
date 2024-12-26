#ifndef EIR_MPU6050
#define EIR_MPU6050

#include <Arduino.h>
#include <Wire.h>
#include "registers.hpp"

// I2C addresses
#define MPU_ADDR_0 0x68
#define MPU_ADDR_1 0x69

#define MPU_ACCEL_ID 0
#define MPU_GYRO_ID 1

class mpu6050
{
private:
    int16_t accel[3];
    int16_t gyro[3];
    int16_t temp;
    int32_t accel_calib[3];
    int32_t gyro_calib[3];
    const uint8_t addr;

public:
    enum IMU_SCALE
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

    enum DLPF_BW
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
    mpu6050(uint8_t _addr);
    mpu6050();
    ~mpu6050();

    /// @brief Wake up the sensor and set default ranges (+-2G, +-250deg/s)
    /// @return True if successful otherwise false
    bool init();
    void calibrate();

    bool read_accel_axis(uint8_t _axis);
    bool read_accel();
    bool read_gyro_axis(uint8_t _axis);
    bool read_gyro();
    bool read_temp();
    bool read_motion();
    bool read_all();

    bool write_accel_scale(IMU_SCALE _scale);
    bool write_gyro_scale(IMU_SCALE _scale);
    bool write_dlpf_bw();

    int16_t get_acc_x() { return accel[0]; };
    int16_t get_acc_y() { return accel[1]; };
    int16_t get_acc_z() { return accel[2]; };
    int16_t get_gyro_x() { return gyro[0]; };
    int16_t get_gyro_y() { return gyro[1]; };
    int16_t get_gyro_z() { return gyro[2]; };
    int16_t get_temp() { return temp; };

private:
    bool write_sensor_scale(uint8_t _sensor, IMU_SCALE _scale);
    bool read_sensor_axis(uint8_t _sensor, uint8_t _axis);
    bool read_sensor(uint8_t _sensor);

    bool write(uint8_t _reg, uint8_t* _data, size_t _len);
    bool write_register(uint8_t _reg, uint8_t _data);
    bool read(uint8_t _reg, uint8_t* _data, size_t _len);
    bool read_register_data8(uint8_t _reg, uint8_t* _data);
    bool read_register_data16(uint8_t _reg, uint16_t* _data);
    bool read_register_data32(uint8_t _reg, uint32_t* _data);
};
#endif // EIR_MPU6050
