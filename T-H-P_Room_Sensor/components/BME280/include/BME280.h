#ifndef _BME2808_H_

#define _BME2808_H_

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "esp_log.h"

#include "i2c_sensor.h"

#define RESET_SENSOR            0xB6

/**********************************
 * I2C Registers
***********************************/
#define HUMIDITY_LSB_REG        0XFE
#define HUMIDITY_MSB_REG        0xFD
#define TEMP_XLSB_REG           0xFC        // only 4 bits (7,6,5,4)
#define TEMP_LSB_REG            0xFB
#define TEMP_MSB_REG            0xFA
#define PRESS_XLSB_REG          0xF9        // only 4 bits (7,6,5,4)
#define PRESS_LSB_REG           0xF8
#define PRESS_MSB_REG           0xF7
#define CONFIG_REGISTER         0xF5        // t_sb (7,6,5) - filter (4,3,2) - spi3w_en (0)     //spi3w_en = SPI 3 wires activate => "0" for disable
#define CTRL_MEAS_REG           0xF4        // osrs_t (7,6,5) - osrs_p (4,3,2) - mode (1,0)
#define STATUS_REG              0xF3        // measuring (3) - im_update (0)
#define CTRL_HUM_REG            0xF2        // osrs_h (3,2,1,0)
#define CALIB_2_REG             0xE1        // 
#define RESET_REG               0xE0
#define CHIP_ID_REG             0xD0        //READ ONLY
#define CALIB_1_REG             0x88        //24 bits + A1

/**********************************
 * Structure et Enumeration
**********************************/

typedef enum
{
    WRITING_OK,
    WRITING_FAIL,
    READING_OK,
    READING_FAIL
} BME280_err_e;

/**
 * @brief Data order
*/
typedef enum {
  press_msb,
  press_lsb,
  press_xlsb,
  temp_msb,
  temp_lsb,
  temp_xlsb,
  hum_msb,
  hum_lsb,
}dataOrder_e;

/**
 * @brief    BME280 mode struct
 * @tparam       NORMAL_CONFIG
 * @tparam       FORCED_CONFIG
 * @tparam       SLEEP_CONFIG
*/
typedef enum
{
    NORMAL_CONFIG   =   0x03,
    FORCED_CONFIG   =   0x01,
    SLEEP_CONFIG    =   0x00
} BME280_mode_e;

/*---------------------------------------------------------------------------------------------------*/

/**
 * @brief    BME280 Oversampling
 * @tparam       Over_0
 * @tparam       Over_1
 * @tparam       Over_2
 * @tparam       Over_4
 * @tparam       Over_8
 * @tparam       Over_16
*/
typedef enum
{
    Over_0          =   0x00,
    Over_1          =   0x01,
    Over_2          =   0x02,
    Over_4          =   0x03,
    Over_8          =   0x04,
    Over_16         =   0x05
} BME280_oversampling_e;

/*---------------------------------------------------------------------------------------------------*/

/**
 * @brief    BME280 filter
 * @tparam       no_filter
 * @tparam       filter_2
 * @tparam       filter_4
 * @tparam       filter_8
 * @tparam       filter_16
*/
typedef enum
{
    no_filter       =   0x00,
    filter_2        =   0x01,
    filter_4        =   0x02,
    filter_8        =   0x03,
    filter_16       =   0x04
} BME280_filter_e;

/*---------------------------------------------------------------------------------------------------*/

/**
 * @brief    BME280 standby time, only on standard mode
 * @tparam       t_sb_0p5
 * @tparam       t_sb_62p5
 * @tparam       t_sb_125
 * @tparam       t_sb_250
 * @tparam       t_sb_500
 * @tparam       t_sb_1000
 * @tparam       t_sb_10
 * @tparam       t_sb_20
*/
typedef enum
{
    t_sb_0p5        =   0x00,
    t_sb_62p5       =   0x01,
    t_sb_125        =   0x02,
    t_sb_250        =   0x03,
    t_sb_500        =   0x04,
    t_sb_1000       =   0x05,
    t_sb_10         =   0x06,
    t_sb_20         =   0x07
} BME280_standby_time_e;

/*---------------------------------------------------------------------------------------------------*/

/**
 * @brief   BME280 protocol : choice between spi 4 or 3 wires
 * @tparam      w4_mode [4 wires spi connection]
 * @tparam      w3_mode [3 wires spi connection]
*/
typedef enum 
{
    w4_mode        =    0x00,
    w3_mode        =    0x01
} BME280_spi3w_en_e;

/*---------------------------------------------------------------------------------------------------*/

/**
 * @brief Compensation value give by the calibration register of the sensor
 * @param dig_T1__3 [Temperature calibration digit of sensor]
 * @param dig_P1__9 [Pressure calibration digit of sensor]
 * @param dig_H1__6 [Humidity calibration digit of sensor]
*/
typedef struct
{
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;  
    int16_t  dig_P6;  
    int16_t  dig_P7; 
    int16_t  dig_P8;
    int16_t  dig_P9;

    unsigned char       dig_H1;
    int16_t             dig_H2;
    unsigned char       dig_H3;
    int16_t             dig_H4;
    int16_t             dig_H5;
    signed char         dig_H6;
} compParam_t;

/*---------------------------------------------------------------------------------------------------*/

/**
 * @brief   BME280 principal configuration with all the controle parameter
 * @param      mode     [SLEEP - FORCED - NORMAL]
 * @param      t_sb     [0.5 - 62.5 - 125 - 250 - 500 - 1000 - 10 000 - 20 000]
 * @param      filter   [no_filter - 2 - 4 - 8 - 16]
 * @param      osrs_T   [0 - 1 - 2 - 4 - 8 - 16]
 * @param      osrs_P   [0 - 1 - 2 - 4 - 8 - 16]
 * @param      osrs_H   [0 - 1 - 2 - 4 - 8 - 16]
 * @param      spi3w    [ 4wires - 3 wires]
*/
typedef struct
{
    BME280_mode_e mode;
    BME280_standby_time_e t_sb;
    BME280_filter_e filter;
    BME280_oversampling_e osrs_T,
                          osrs_P,
                          osrs_H;
    BME280_spi3w_en_e spi3w;     
} BME280_config_t;

/**********************************
 * BME280 functions
**********************************/

/**
 * @brief Configure all the sensor function and requirements to work
 * @param sensor            [I2C bus configuration]
 * @param config            [Sensor config with : mode, standby_time, filter, osrs_T, osrs_P, osrs_H]
*/
esp_err_t BME280_config(i2c_sensor_t sensor, BME280_config_t config);

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_mode(i2c_sensor_t sensor, BME280_mode_e mode);

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_TimeStandby(i2c_sensor_t sensor, BME280_standby_time_e time);

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_filtercoef(i2c_sensor_t sensor, BME280_filter_e filter_coef);

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_oversamp_Temp(i2c_sensor_t sensor, BME280_oversampling_e osrs_T);

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_oversamp_Press(i2c_sensor_t sensor, BME280_oversampling_e osrs_P);

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_oversamp_Hum(i2c_sensor_t sensor, BME280_oversampling_e osrs_H);

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_Reset_sensor(i2c_sensor_t sensor);

/**********************************
 * BME280 return functions
**********************************/
/***
 * @brief 
 * @return -1 error, 
 * @return 0 Measure finished, 
 * @return 1 Measure at the moment
*/
int8_t BME280_Measuring(i2c_sensor_t sensor);

/*---------------------------------------------------------------------------------------------------*/

int8_t BME280_Im_Update(i2c_sensor_t sensor);

/*---------------------------------------------------------------------------------------------------*/

int8_t BME280_Read_Chip_ID(i2c_sensor_t sensor);

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_Read_Compensation(i2c_sensor_t sensor);

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_Read_T_P_H_values(i2c_sensor_t sensor, int32_t *Raw_values);

/*---------------------------------------------------------------------------------------------------*/

/**
 * @brief Return Temperature values in degC
 * @param adc_T [Temperature raw value]
 * @return Returns temperature in DegC, double precision. Output value of “51.23” equals 51.23 DegC.
*/
double BME280_compensate_T_double(int32_t adc_T);

/*---------------------------------------------------------------------------------------------------*/

/**
 * @brief Return Pressure values in Pa
 * @param adc_P [Pressure raw value]
 * @return Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa
*/
double BME280_compensate_P_double(int32_t adc_P);

/*---------------------------------------------------------------------------------------------------*/

/**
 * @brief Return humidity values in pourcentage
 * @param adc_H [Humidity raw value]
 * @return Returns humidity in %rH as as double. Output value of “46.332” represents 46.332 % rH 
*/
double BME280_compensate_H_double(int32_t adc_H);

const char* BME280_err_to_name(BME280_err_e err_code);
#endif