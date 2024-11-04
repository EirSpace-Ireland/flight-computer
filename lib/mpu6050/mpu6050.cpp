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
    Wire.beginTransmission(addr);
    Wire.write(REG_WHO_AM_I);
    if (Wire.endTransmission() != 0)
    {
        // Serial.println(F("mpu6050 init err"));
        return false;
    }

    if (Wire.requestFrom(addr, (uint8_t)1) != 1)
    {
        // Serial.println(F("mpu6050 init err"));
        return false;
    }

    uint8_t res = Wire.read();
    if (res != MPU_ADDR_0)
    {
        // Serial.println(F("whoami err"));
        return false;
    }

    // Serial.print(F("WHOAMI: "));
    // Serial.println(res, HEX);

    // todo extract
    // Reset MPU
    Wire.beginTransmission(addr);
    Wire.write(REG_PWR_MGMT_1);
    Wire.write(1 << 7);
    if (Wire.endTransmission() != 0)
    {
        // Serial.println(F("mpu6050 init err"));
        return false;
    }
    delay(100);

    // Turn on MPU
    Wire.beginTransmission(addr);
    Wire.write(REG_PWR_MGMT_1);
    Wire.write(0);
    if (Wire.endTransmission() != 0)
    {
        // Serial.println(F("mpu6050 init err"));
        return false;
    }
    return true;
}

bool mpu6050::read_accel_axis(uint8_t _axis)
{
    return read_sensor_axis(MPU_ACCEL_ID, _axis);
}

bool mpu6050::read_accel()
{
    return read_sensor(MPU_ACCEL_ID);
}

bool mpu6050::read_gyro()
{
    return read_sensor(MPU_GYRO_ID);
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
        Serial.println("INVALED AXIS");
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

bool mpu6050::read_temp()
{
    Wire.beginTransmission(addr);
    Wire.write(REG_TEMP_OUT_H);
    if (Wire.endTransmission() != 0)
    {
        //Serial.println(F("read_temp err"));
        return false;
    }
    if (Wire.requestFrom(addr, (uint8_t)2) != 2)
    {
        //Serial.println(F("read_temp err"));
        return false;
    }
    temp = Wire.read() << 8 | Wire.read();
    return true;
}

bool mpu6050::read_motion()
{
    if (read_accel() == false || read_gyro() == false)
    {
        // Serial.println(F("read_motion err"));
        return false;
    }
    return true;
}

bool mpu6050::read_all()
{
    if (read_accel() == false || read_gyro() == false || read_temp() == false)
    {
        // Serial.println(F("read_all err"));
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
