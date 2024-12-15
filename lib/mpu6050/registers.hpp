#ifndef EIR_MPU6051_REG
#define EIR_MPU6050_REG

// REGISTER DEFINITIONS
#define REG_0 0                    // Reg 0 - RESERVED
#define REG_1 1                    // Reg 1 - RESERVED
#define REG_2 2                    // Reg 2 - RESERVED
#define REG_3 3                    // Reg 3 - RESERVED
#define REG_4 4                    // Reg 4 - RESERVED
#define REG_5 5                    // Reg 5 - RESERVED
#define REG_6 6                    // Reg 6 - RESERVED
#define REG_7 7                    // Reg 7 - RESERVED
#define REG_8 8                    // Reg 8 - RESERVED
#define REG_9 9                    // Reg 9 - RESERVED
#define REG_10 10                  // Reg 10 - RESERVED
#define REG_11 11                  // Reg 11 - RESERVED
#define REG_12 12                  // Reg 12 - RESERVED
#define REG_SELF_TEST_X 13         // Reg 13
#define REG_SELF_TEST_Y 14         // Reg 14
#define REG_SELF_TEST_Z 15         // Reg 15
#define REG_SELF_TEST_A 16         // Reg 16
#define REG_17 17                  // Reg 17 - RESERVED
#define REG_18 18                  // Reg 18 - RESERVED
#define REG_19 19                  // Reg 19 - RESERVED
#define REG_20 20                  // Reg 20 - RESERVED
#define REG_21 21                  // Reg 21 - RESERVED
#define REG_22 22                  // Reg 22 - RESERVED
#define REG_23 23                  // Reg 23 - RESERVED
#define REG_24 24                  // Reg 24 - RESERVED
#define REG_SMPLRT_DIV 25          // Reg 25
#define REG_CONFIG 26              // Reg 26
#define REG_GYRO_CONFIG 27         // Reg 27
#define REG_ACCEL_CONFIG 28        // Reg 28
#define REG_29 29                  // Reg 29 - RESERVED
#define REG_30 30                  // Reg 30 - RESERVED
#define REG_MOT_THR 31             // Reg 31
#define REG_32 32                  // Reg 32 - RESERVED
#define REG_33 33                  // Reg 33 - RESERVED
#define REG_34 34                  // Reg 34 - RESERVED
#define REG_FIFO_EN 35             // Reg 35
#define REG_I2C_MST_CTRL 36        // Reg 36
#define REG_I2C_SLV0_ADDR 37       // Reg 37
#define REG_I2C_SLV0_REG 38        // Reg 38
#define REG_I2C_SLV0_CTRL 39       // Reg 39
#define REG_I2C_SLV1_ADDR 40       // Reg 40
#define REG_I2C_SLV1_REG 41        // Reg 41
#define REG_I2C_SLV1_CTRL 42       // Reg 42
#define REG_I2C_SLV2_ADDR 43       // Reg 43
#define REG_I2C_SLV2_REG 44        // Reg 44
#define REG_I2C_SLV2_CTRL 45       // Reg 45
#define REG_I2C_SLV3_ADDR 46       // Reg 46
#define REG_I2C_SLV3_REG 47        // Reg 47
#define REG_I2C_SLV3_CTRL 48       // Reg 48
#define REG_I2C_SLV4_ADDR 49       // Reg 49
#define REG_I2C_SLV4_REG 50        // Reg 50
#define REG_I2C_SLV4_DO 51         // Reg 51
#define REG_I2C_SLV4_CTRL 52       // Reg 52
#define REG_I2C_SLV4_DI 53         // Reg 53
#define REG_I2C_MST_STATUS 54      // Reg 54
#define REG_INT_PIN_CFG 55         // Reg 55
#define REG_INT_ENABLE 56          // Reg 56
#define REG_57 57                  // Reg 57 - RESERVED
#define REG_INT_STATUS 58          // Reg 58
#define REG_ACCEL_XOUT_H 59        // Reg 59
#define REG_ACCEL_XOUT_L 60        // Reg 60
#define REG_ACCEL_YOUT_H 61        // Reg 61
#define REG_ACCEL_YOUT_L 62        // Reg 62
#define REG_ACCEL_ZOUT_H 63        // Reg 63
#define REG_ACCEL_ZOUT_L 64        // Reg 64
#define REG_TEMP_OUT_H 65          // Reg 65
#define REG_TEMP_OUT_L 66          // Reg 66
#define REG_GYRO_XOUT_H 67         // Reg 67
#define REG_GYRO_XOUT_L 68         // Reg 68
#define REG_GYRO_YOUT_H 69         // Reg 69
#define REG_GYRO_YOUT_L 70         // Reg 70
#define REG_GYRO_ZOUT_H 71         // Reg 71
#define REG_GYRO_ZOUT_L 72         // Reg 72
#define REG_EXT_SENS_DATA_0 73     // Reg 73
#define REG_EXT_SENS_DATA_1 74     // Reg 74
#define REG_EXT_SENS_DATA_2 75     // Reg 75
#define REG_EXT_SENS_DATA_3 76     // Reg 76
#define REG_EXT_SENS_DATA_4 77     // Reg 77
#define REG_EXT_SENS_DATA_5 78     // Reg 78
#define REG_EXT_SENS_DATA_6 79     // Reg 79
#define REG_EXT_SENS_DATA_7 80     // Reg 80
#define REG_EXT_SENS_DATA_8 81     // Reg 81
#define REG_EXT_SENS_DATA_9 82     // Reg 82
#define REG_EXT_SENS_DATA_10 83    // Reg 83
#define REG_EXT_SENS_DATA_11 84    // Reg 84
#define REG_EXT_SENS_DATA_12 85    // Reg 85
#define REG_EXT_SENS_DATA_13 86    // Reg 86
#define REG_EXT_SENS_DATA_14 87    // Reg 87
#define REG_EXT_SENS_DATA_15 88    // Reg 88
#define REG_EXT_SENS_DATA_16 89    // Reg 89
#define REG_EXT_SENS_DATA_17 90    // Reg 90
#define REG_EXT_SENS_DATA_18 91    // Reg 91
#define REG_EXT_SENS_DATA_19 92    // Reg 92
#define REG_EXT_SENS_DATA_20 93    // Reg 93
#define REG_EXT_SENS_DATA_21 94    // Reg 94
#define REG_EXT_SENS_DATA_22 95    // Reg 95
#define REG_EXT_SENS_DATA_23 96    // Reg 96
#define REG_97 97                  // Reg 97 - RESERVED
#define REG_98 98                  // Reg 98 - RESERVED
#define REG_SLV0_DO 99             // Reg 99
#define REG_SLV1_DO 100            // Reg 100
#define REG_SLV2_DO 101            // Reg 101
#define REG_SLV3_DO 102            // Reg 102
#define REG_I2C_MST_DELAY_CTRL 103 // Reg 103
#define REG_SIGNAL_PATH_RESET 104  // Reg 104
#define REG_MOT_DETECT_CTRL 105    // Reg 105
#define REG_USER_CTRL 106          // Reg 106
#define REG_PWR_MGMT_1 107         // Reg 107
#define REG_PWR_MGMT_2 108         // Reg 108
#define REG_109 109                // Reg 109 - RESERVED
#define REG_110 110                // Reg 110 - RESERVED
#define REG_111 111                // Reg 111 - RESERVED
#define REG_112 112                // Reg 112 - RESERVED
#define REG_113 113                // Reg 113 - RESERVED
#define REG_FIFO_COUNTH 114        // Reg 114
#define REG_FIFO_COUNTL 115        // Reg 115
#define REG_FIFO_R_W 116           // Reg 116
#define REG_WHO_AM_I 117           // Reg 117

#endif // EIR_MPU6051_REG
