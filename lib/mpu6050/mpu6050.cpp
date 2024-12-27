#include "mpu6050.hpp"

mpu6050::mpu6050()
    : addr{MPU_ADDR_0}
{
}

mpu6050::mpu6050(uint8_t _addr)
    : addr{_addr}
{
    // if (_addr != MPU_ADDR_0 && _addr != MPU_ADDR_1)
    //{
    //     addr = MPU_ADDR_0;
    // }
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

    delay(100); // As per register map pg 42
    return true;
}

bool mpu6050::read_accel_axis(uint8_t _axis)
{
    return read_sensor_axis(MPU_ACCEL_ID, _axis);
}

bool mpu6050::read_accel()
{
    // todo: fix this to just use mpu6050::read
    mpu6050::read_register_data16(REG_ACCEL_XOUT_H, reinterpret_cast<uint16_t*>(&accel[0]));
    mpu6050::read_register_data16(REG_ACCEL_YOUT_H, reinterpret_cast<uint16_t*>(&accel[1]));
    mpu6050::read_register_data16(REG_ACCEL_ZOUT_H, reinterpret_cast<uint16_t*>(&accel[2]));
    //uint8_t data[6] {0};
    ///***
    // * Remember: read fills data buffer back to front (to allow for uintXX_t filling)
    // * 0 zl
    // * 1 zh
    // * 2 yl
    // * 3 yh
    // * 4 xl
    // * 5 xh
    // */
    //if(!mpu6050::read(REG_ACCEL_XOUT_H, data, 6))
    //{
    //    return false;
    //}
    //accel[0] = (data[1] << 8) | data[0];
    //accel[1] = (data[3] << 8) | data[2];
    //accel[2] = (data[5] << 8) | data[4];
    return true;
    //return read_sensor(MPU_ACCEL_ID);
}

bool mpu6050::read_gyro()
{
    // todo: fix this to just use mpu6050::read
    mpu6050::read_register_data16(REG_GYRO_XOUT_H, reinterpret_cast<uint16_t*>(&gyro[0]));
    mpu6050::read_register_data16(REG_GYRO_YOUT_H, reinterpret_cast<uint16_t*>(&gyro[1]));
    mpu6050::read_register_data16(REG_GYRO_ZOUT_H, reinterpret_cast<uint16_t*>(&gyro[2]));
    // uint8_t data[6] {0};
    // if(!mpu6050::read(REG_ACCEL_XOUT_H, data, 6))
    // {
    //     return false;
    // }
    // gyro[0] = (data[1] << 8) | data[0];
    // gyro[1] = (data[3] << 8) | data[2];
    // gyro[2] = (data[5] << 8) | data[4];
    return true;
    //return read_sensor(MPU_GYRO_ID);
}

bool mpu6050::read_sensor_axis(uint8_t _sensor, uint8_t _axis)
{
    uint8_t gyro_reg_offset;
    int16_t* out_arr;
    if (_sensor == MPU_ACCEL_ID)
    {
        gyro_reg_offset = 0;
        out_arr = accel;
    }
    else
    {
        gyro_reg_offset = 8;
        out_arr = gyro;
    }


    Wire.beginTransmission(addr);
    switch (_axis)
    {
    case 0: // x axis
        if (Wire.write(REG_ACCEL_XOUT_H + gyro_reg_offset) == 0)
        {
            Serial.println("I2C write error");
            return false;
        }
        break;

    case 1: // y axis
        if (Wire.write(REG_ACCEL_YOUT_H + gyro_reg_offset) == 0)
        {
            Serial.println("I2C write error");
            return false;
        }
        break;

    case 2: // z axis
        if (Wire.write(REG_ACCEL_ZOUT_H + gyro_reg_offset) == 0)
        {
            Serial.println("I2C write error");
            return false;
        }
        break;

    default:
        Serial.println("INVALID AXIS");
        return false;
        break;
    }

    Wire.endTransmission();
    Wire.requestFrom(addr, (uint8_t)2);
    out_arr[_axis] = Wire.read() << 8 | Wire.read();
    return true;
}

bool mpu6050::read_gyro_axis(uint8_t _axis)
{
    return read_sensor_axis(MPU_GYRO_ID, _axis);
}

bool mpu6050::read_sensor(uint8_t _sensor)
{
    uint8_t read_reg;
    int16_t *out_arr;
    switch (_sensor)
    {
    case MPU_ACCEL_ID:
        read_reg = REG_ACCEL_XOUT_H;
        out_arr = accel;
        break;
    case MPU_GYRO_ID:
        read_reg = REG_GYRO_XOUT_H;
        out_arr = gyro;
        break;

    default:
        break;
    }
    Wire.beginTransmission(addr);
    Wire.write(read_reg);
    Wire.endTransmission();
    Wire.requestFrom(addr, (uint8_t)6);

    for (size_t i = 0; i < 3; i++)
    {
        out_arr[i] = Wire.read() << 8 | Wire.read();
    }

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
    Wire.beginTransmission(this->addr);

    if (Wire.write(_reg) != 1)
    {
        return false;
    }

    if (Wire.write(_data) != 1)
    {
        return false;
    }

    if (Wire.endTransmission() != 0)
    {
        return false;
    }

    return true;
}

bool mpu6050::read(uint8_t _reg, uint8_t *_data, size_t _len)
{
    Wire.beginTransmission(this->addr);

    if (Wire.write(_reg) != 1)
    {
        Serial.println("I2C write err mpu6050::read");
        return false;
    }

    Wire.endTransmission();

    while (Wire.available())
    {
        Wire.read();
    }

    if (Wire.requestFrom(this->addr, _len) != _len)
    {
        Serial.println("I2C read err. mpu6050::read");
        return false;
    }

    // MSB first
    for (size_t i = (_len - 1); i >= 0 && i != SIZE_MAX; i--)
    {
        _data[i] = Wire.read();
    }

    return true;
}

bool mpu6050::read_register_data8(uint8_t _reg, uint8_t *_data)
{
    return mpu6050::read(_reg, _data, 1);
}

bool mpu6050::read_register_data16(uint8_t _reg, uint16_t *_data)
{
    return mpu6050::read(_reg, reinterpret_cast<uint8_t*>(_data), 2);
}

bool mpu6050::read_register_data32(uint8_t _reg, uint32_t *_data)
{
    return mpu6050::read(_reg, reinterpret_cast<uint8_t*>(_data), 4);
}

bool mpu6050::read_temp()
{
    if (!read_register_data16(REG_TEMP_OUT_H, reinterpret_cast<uint16_t*>(&temp)))
    {
        Serial.println("read temp err");
        return false;
    }

    return true;
}

bool mpu6050::read_motion()
{
    if (read_accel() == false || read_gyro() == false)
    {
        Serial.println(F("read_motion err"));
        return false;
    }
    return true;
}

bool mpu6050::read_all()
{
    if (read_accel() == false || read_gyro() == false || read_temp() == false)
    {
        Serial.println(F("read_all err"));
        return false;
    }
    return true;
}

bool mpu6050::write_sensor_scale(uint8_t _sensor, IMU_SCALE _scale)
{
    uint8_t write_reg;
    switch (_sensor)
    {
    case MPU_ACCEL_ID:
        write_reg = REG_ACCEL_CONFIG;
        break;

    case MPU_GYRO_ID:
        write_reg = REG_GYRO_CONFIG;
        break;

    default:
        // Serial.println("Invalid switch expression at: ");
        // Serial.print(__FILE__);
        // Serial.println(__LINE__);
        break;
    }
    // Serial.println("TRACE write_accel_scale");
    Wire.beginTransmission(addr);
    Wire.write(write_reg);
    if (Wire.endTransmission() != 0)
    {
        // Serial.println(F("I2C Err"));
        return false;
    }

    if (Wire.requestFrom(addr, (uint8_t)1) != 1)
    {
        // Serial.println(F("I2C Err"));
        return false;
    }

    uint8_t state = Wire.read();
    // Serial.print("TRACE init value: ");
    // Serial.println(state, BIN);

    state &= 0b11100111; // Clear the bits we want to set
    state |= _scale << 3;

    // Serial.print("TRACE new state");
    // Serial.println(state, BIN);

    Wire.beginTransmission(addr);
    Wire.write(write_reg);
    Wire.write(state | (_scale << 3));
    if (Wire.endTransmission() != 0)
    {
        // Serial.println(F("I2C Err"));
        return false;
    }

    return true;
}

bool mpu6050::write_accel_scale(IMU_SCALE _scale)
{
    return write_sensor_scale(MPU_ACCEL_ID, _scale);
}

bool mpu6050::write_gyro_scale(IMU_SCALE _scale)
{
    return write_sensor_scale(MPU_GYRO_ID, _scale);
}

bool mpu6050::write_dlpf_bw()
{
    return false;
}
