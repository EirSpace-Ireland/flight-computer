#include "mpu6050.hpp"
#include "registers.hpp"

// I2C addresses
#define MPU_ADDR_0 0x68

// Conversion factors
#define ACCEL_CONV_2G 16384.0f
#define ACCEL_CONV_4G 8192.0f
#define ACCEL_CONV_8G 4096.0f
#define ACCEL_CONV_16G 2048.0f

#define GYRO_CONV_250 131.0f
#define GYRO_CONV_500 65.5f
#define GYRO_CONV_1000 32.8f
#define GYRO_CONV_2000 16.4f

mpu6050::mpu6050()
    : addr{MPU_ADDR_0},
      accel_scale_factors{ACCEL_CONV_2G, ACCEL_CONV_4G, ACCEL_CONV_8G, ACCEL_CONV_16G},
      gyro_scale_factors{GYRO_CONV_250, GYRO_CONV_500, GYRO_CONV_1000, GYRO_CONV_2000},
      accel_conv{ACCEL_CONV_2G},
      gyro_conv{GYRO_CONV_250}
{
}

mpu6050::mpu6050(bool _addr_0)
    : addr{MPU_ADDR_0 | _addr_0},
      accel_scale_factors{ACCEL_CONV_2G, ACCEL_CONV_4G, ACCEL_CONV_8G, ACCEL_CONV_16G},
      gyro_scale_factors{GYRO_CONV_250, GYRO_CONV_500, GYRO_CONV_1000, GYRO_CONV_2000},
      accel_conv{ACCEL_CONV_2G},
      gyro_conv{GYRO_CONV_250}
{
}

mpu6050::~mpu6050()
{
}

bool mpu6050::init()
{
    // Validate whoami register
    uint8_t result = 0;
    if (!read_register_data8(REG_WHO_AM_I, &result))
    {
        return false;
    }

    if (result != MPU_ADDR_0)
    {
        // Serial.println(F("whoami err"));
        return false;
    }

    // Serial.print(F("WHOAMI: "));
    // Serial.println(res, HEX);
    mpu6050::reset();
    delay(100); // As per register map pg 42

    // Turn on MPU
    if (!write_register(REG_PWR_MGMT_1, 0))
    {
        return false;
    }

    return true;
}

bool mpu6050::reset()
{
    if (!write_register(REG_PWR_MGMT_1, (1 << 7))) // Device reset bit
    {
        return false;
    }

    return true;
}

bool mpu6050::read_accel_axis(IMU_AXIS _axis)
{
    return read_sensor_axis(ACCEL, _axis);
}

bool mpu6050::read_accel()
{
    return read_sensor(ACCEL);
}

bool mpu6050::read_gyro()
{
    return read_sensor(GYRO);
}

bool mpu6050::read_sensor_axis(SENSOR_TYPE _sensor, IMU_AXIS _axis)
{
    uint8_t sensor_reg_offset;
    uint16_t out_value;
    int16_t *out_arr;

    // Accelerometer and gyro registers have the same layout but are offset by 8
    // in the register map. The output array is also different
    sensor_reg_offset = (_sensor == ACCEL) ? 0 : 8;
    out_arr = (_sensor == ACCEL) ? accel_raw : gyro_raw;

    switch (_axis)
    {
    case IMU_X:
        if (!read_register_data16((REG_ACCEL_XOUT_H + sensor_reg_offset), &out_value))
        {
            return false;
        }
        break;

    case IMU_Y:
        if (!read_register_data16((REG_ACCEL_YOUT_H + sensor_reg_offset), &out_value))
        {
            return false;
        }
        break;

    case IMU_Z:
        if (!read_register_data16((REG_ACCEL_ZOUT_H + sensor_reg_offset), &out_value))
        {
            return false;
        }
        break;
    }

    out_arr[_axis] = out_value;
    return true;
}

bool mpu6050::read_gyro_axis(IMU_AXIS _axis)
{
    return read_sensor_axis(GYRO, _axis);
}

bool mpu6050::read_sensor(SENSOR_TYPE _sensor)
{
    uint8_t read_reg;
    int16_t *out_arr;
    if (_sensor == ACCEL)
    {
        read_reg = REG_ACCEL_XOUT_H;
        out_arr = accel_raw;
    }
    else
    {
        read_reg = REG_GYRO_XOUT_H;
        out_arr = gyro_raw;
    }

    uint8_t data[6]{0};
    if (!mpu6050::read(read_reg, data, 6))
    {
        return false;
    }

    out_arr[IMU_X] = (data[5] << 8) | data[4];
    out_arr[IMU_Y] = (data[3] << 8) | data[2];
    out_arr[IMU_Z] = (data[1] << 8) | data[0];

    return true;
}

bool mpu6050::write(uint8_t _reg, uint8_t *_data, size_t _len)
{
    Wire.beginTransmission(this->addr);
    if (Wire.write(_reg) != 1)
    {
        return false;
    }

    // Amount of data written should match what we expect
    if (Wire.write(_data, _len) != _len)
    {
        Wire.endTransmission();
        return false;
    }

    if (Wire.endTransmission() != 0)
    {
        return false;
    }

    return true;
}

bool mpu6050::write_register(uint8_t _reg, uint8_t _data)
{
    return mpu6050::write(_reg, &_data, sizeof(uint8_t));
}

bool mpu6050::read(uint8_t _reg, uint8_t *_data, size_t _len)
{
    Wire.beginTransmission(this->addr);

    if (Wire.write(_reg) != 1)
    {
        return false;
    }

    Wire.endTransmission();

    // Empty buffer
    while (Wire.available())
    {
        Wire.read();
    }

    if (Wire.requestFrom(this->addr, _len) != _len)
    {
        return false;
    }

    // MSB first (in case the destination is a single integer of variable width)
    for (size_t i = (_len - 1); i != SIZE_MAX; i--)
    {
        _data[i] = Wire.read();
    }

    return true;
}

bool mpu6050::read_register_data8(uint8_t _reg, uint8_t *_data)
{
    return mpu6050::read(_reg, _data, sizeof(uint8_t));
}

bool mpu6050::read_register_data16(uint8_t _reg, uint16_t *_data)
{
    return mpu6050::read(_reg, reinterpret_cast<uint8_t *>(_data), sizeof(uint16_t));
}

bool mpu6050::read_register_data32(uint8_t _reg, uint32_t *_data)
{
    return mpu6050::read(_reg, reinterpret_cast<uint8_t *>(_data), sizeof(uint32_t));
}

bool mpu6050::read_temp()
{
    if (!read_register_data16(REG_TEMP_OUT_H, reinterpret_cast<uint16_t *>(&temp)))
    {
        return false;
    }

    return true;
}

bool mpu6050::read_motion()
{
    if (read_accel() == false || read_gyro() == false)
    {
        return false;
    }
    return true;
}

bool mpu6050::read_all()
{
    if (read_accel() == false || read_gyro() == false || read_temp() == false)
    {
        return false;
    }
    return true;
}

bool mpu6050::write_sensor_scale(SENSOR_TYPE _sensor, IMU_SCALE _scale)
{
    uint8_t reg_ptr;
    uint8_t reg_state;
    const uint8_t config_mask = 0b00011000;
    if (_sensor == ACCEL)
    {
        reg_ptr = REG_ACCEL_CONFIG;
        accel_conv = accel_scale_factors[_scale];
    }
    else
    {
        reg_ptr = REG_GYRO_CONFIG;
        gyro_conv = gyro_scale_factors[_scale];
    }

    if (!read_register_data8(reg_ptr, &reg_state))
    {
        return false;
    }

    // Set the scale bits to 0 and then set the new scale
    reg_state &= static_cast<uint8_t>(~(config_mask));
    reg_state |= _scale << 3;

    if (!write_register(reg_ptr, reg_state))
    {
        return false;
    }

    return true;
}

bool mpu6050::write_accel_scale(IMU_SCALE _scale)
{
    return write_sensor_scale(ACCEL, _scale);
}

bool mpu6050::write_gyro_scale(IMU_SCALE _scale)
{
    return write_sensor_scale(GYRO, _scale);
}

bool mpu6050::write_dlpf_bw()
{
    return false;
}

bool mpu6050::offset_calibration(const uint16_t num_iterations)
{
    int32_t accl_accum_x = 0;
    int32_t accl_accum_y = 0;
    int32_t accl_accum_z = 0;
    int32_t gyro_accum_x = 0;
    int32_t gyro_accum_y = 0;
    int32_t gyro_accum_z = 0;

    // accum num_iterations values for each axis
    for (size_t i = 0; i < num_iterations; i++)
    {
        this->read_all();

        accl_accum_x += this->get_acc_x_raw();
        accl_accum_y += this->get_acc_y_raw();
        accl_accum_z += this->get_acc_z_raw();
        gyro_accum_x += this->get_gyro_x_raw();
        gyro_accum_y += this->get_gyro_y_raw();
        gyro_accum_z += this->get_gyro_z_raw();
    }

    // get average
    // store in offset valiables
    accel_offset[IMU_X] = accl_accum_x / num_iterations;
    accel_offset[IMU_Y] = accl_accum_y / num_iterations;
    accel_offset[IMU_Z] = (accl_accum_z / num_iterations) - accel_conv;
    gyro_offset[IMU_X] = gyro_accum_x / num_iterations;
    gyro_offset[IMU_Y] = gyro_accum_y / num_iterations;
    gyro_offset[IMU_Z] = gyro_accum_z / num_iterations;

    return true;
}
