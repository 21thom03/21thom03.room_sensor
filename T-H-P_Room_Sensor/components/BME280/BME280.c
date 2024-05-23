#include <stdio.h>
#include "BME280.h"

/******************************
 * Globales variables
 ******************************/
static const char *TAG = "BME280";

compParam_t CompenParam;

/**
 * @brief t_fine carries fine temperature as global value
 */
double t_fine;

/******************************
 * BME280 Sensor functions
 ******************************/

BME280_err_e BME280_write_mode(i2c_sensor_t sensor, BME280_mode_e mode)
{
    uint8_t mode_config = 0;
    esp_err_t ret = i2c_read_bit(sensor, CTRL_MEAS_REG,&mode_config);

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Mode number: %d, Status: %s\r", mode, BME280_err_to_name(READING_FAIL));
        return READING_FAIL;
    }

    mode_config &= 0b11111100;
    mode_config |= mode;

    ret = i2c_write_bit(sensor, CTRL_MEAS_REG, &mode_config);

    if( ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Mode number: %d, Status: %s\r", mode, BME280_err_to_name(WRITING_FAIL));
        return WRITING_FAIL;
    }else{
        ESP_LOGW(TAG, "Mode number: %d, Status: %s\r", mode, BME280_err_to_name(WRITING_OK));
        return WRITING_OK;
    }

}

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_TimeStandby(i2c_sensor_t sensor, BME280_standby_time_e time)
{
    uint8_t time_config = 0;
    esp_err_t ret = i2c_read_bit(sensor,CONFIG_REGISTER , &time_config);

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Standby time mode: %d, Status: %s\r", time, BME280_err_to_name(READING_FAIL));
        return READING_FAIL;
    }
    
    time_config &= 0b00011111;
    time_config |= time << 5;

    ret = i2c_write_bit(sensor, CONFIG_REGISTER, &time_config);

    if( ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Standby time mode: %d, Status: %s\r", time, BME280_err_to_name(WRITING_FAIL));
        return WRITING_FAIL;
    }else{
        ESP_LOGW(TAG, "Standby time mode: %d, Status: %s\r", time, BME280_err_to_name(WRITING_OK));
        return WRITING_OK;
    }
}

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_filtercoef(i2c_sensor_t sensor, BME280_filter_e filter_coef)
{
    uint8_t filter_config = 0;
    esp_err_t ret = i2c_read_bit(sensor,CONFIG_REGISTER , &filter_config);

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Filter mode: x%f, Status: %s\r", pow(filter_coef,2), BME280_err_to_name(READING_FAIL));
        return READING_FAIL;
    }
    
    filter_config &= 0b11100011;
    filter_config |= filter_coef << 2;

    ret = i2c_write_bit(sensor, CONFIG_REGISTER, &filter_config);

    if( ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Filter mode: x%f, Status: %s\r", pow(filter_coef,2), BME280_err_to_name(WRITING_FAIL));
        return WRITING_FAIL;
    }else{
        ESP_LOGW(TAG, "Filter mode: x%f, Status: %s\r", pow(filter_coef,2), BME280_err_to_name(WRITING_OK));
        return WRITING_OK;
    }
}

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_oversamp_Temp(i2c_sensor_t sensor, BME280_oversampling_e osrs_T)
{
    uint8_t os_Temp_config = 0;
    esp_err_t ret = i2c_read_bit(sensor,CTRL_MEAS_REG , &os_Temp_config);

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Temperature OverSampling mode: x%f, Status: %s\r", pow(osrs_T,2), BME280_err_to_name(READING_FAIL));
        return READING_FAIL;
    }
    
    os_Temp_config &= 0b00011111;
    os_Temp_config |= osrs_T << 5;

    ret = i2c_write_bit(sensor, CTRL_MEAS_REG, &os_Temp_config);

    if( ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Temperature OverSampling mode: x%f, Status: %s\r", pow(osrs_T,2), BME280_err_to_name(WRITING_FAIL));
        return WRITING_FAIL;
    }else{
        ESP_LOGW(TAG, "Temperature OverSampling mode: x%f, Status: %s\r", pow(osrs_T,2), BME280_err_to_name(WRITING_OK));
        return WRITING_OK;
    }
}

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_oversamp_Press(i2c_sensor_t sensor, BME280_oversampling_e osrs_P)
{
    uint8_t os_Press_config = 0;
    esp_err_t ret = i2c_read_bit(sensor,CTRL_MEAS_REG , &os_Press_config);

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Temperature OverSampling mode: x%f, Status: %s\r", pow(osrs_P,2), BME280_err_to_name(READING_FAIL));
        return READING_FAIL;
    }
    
    os_Press_config &= 0b11100011;
    os_Press_config |= osrs_P << 2;

    ret = i2c_write_bit(sensor, CTRL_MEAS_REG, &os_Press_config);

    if( ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Temperature OverSampling mode: x%f, Status: %s\r", pow(osrs_P,2), BME280_err_to_name(WRITING_FAIL));
        return WRITING_FAIL;
    }else{
        ESP_LOGW(TAG, "Temperature OverSampling mode: x%f, Status: %s\r", pow(osrs_P,2), BME280_err_to_name(WRITING_OK));
        return WRITING_OK;
    }
}

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_write_oversamp_Hum(i2c_sensor_t sensor, BME280_oversampling_e osrs_H)
{
    uint8_t os_Hum_config = 0;
    esp_err_t ret = i2c_read_bit(sensor,CTRL_HUM_REG , &os_Hum_config);

    if(ret != ESP_OK)
    {

        ESP_LOGW(TAG, "Temperature OverSampling mode: x%f, Status: %s\r", pow(osrs_H,2), BME280_err_to_name(READING_FAIL));
        return READING_FAIL;
    }
    
    os_Hum_config &= 0b11111100;
    os_Hum_config |= osrs_H;

    ret = i2c_write_bit(sensor, CTRL_HUM_REG, &os_Hum_config);

    if( ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Temperature OverSampling mode: x%f, Status: %s\r", pow(osrs_H,2), BME280_err_to_name(WRITING_FAIL));
        return WRITING_FAIL;
    }else{
        ESP_LOGW(TAG, "Temperature OverSampling mode: x%f, Status: %s\r", pow(osrs_H,2), BME280_err_to_name(WRITING_OK));
        return WRITING_OK;
    }
}

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_Reset_sensor(i2c_sensor_t sensor)
{
    uint8_t reset = RESET_SENSOR;
    esp_err_t ret = i2c_write_bit(sensor,RESET_REG, &reset);

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Reset sensor config failed, Status: %s\r", BME280_err_to_name(WRITING_FAIL));
        return WRITING_FAIL;
    }else{
        ESP_LOGW(TAG, "Reset sensor config OK, Status: %s\r", BME280_err_to_name(WRITING_OK));
        return WRITING_OK;
    }
}

/*---------------------------------------------------------------------------------------------------*/

esp_err_t BME280_config(i2c_sensor_t sensor, BME280_config_t config)
{
    BME280_err_e ret;
    // uint8_t config_data = config.t_sb << 5 | config.filter << 2 | config.spi3w;
    // uint8_t ctrl_meas_data = config.osrs_T << 5 | config.osrs_P << 2 | config.mode;
    // uint8_t ctrl_hum_data = config.osrs_H;

    ret = BME280_write_mode(sensor, config.mode);
    ret = BME280_write_filtercoef(sensor, config.filter);
    ret = BME280_write_TimeStandby(sensor, config.t_sb);
    ret = BME280_write_oversamp_Temp(sensor, config.osrs_T);
    ret = BME280_write_oversamp_Press(sensor, config.osrs_P);
    ret = BME280_write_oversamp_Hum(sensor, config.osrs_H);

    if(ret != READING_FAIL || ret != WRITING_FAIL)
    {
        ESP_LOGW(TAG, "/-------Sensor configuration finished---------/");
        ESP_LOGW(TAG, "Mode : %d\r", config.mode);
        ESP_LOGW(TAG, "Filter coefficient : %d, Standby time : %d\r", config.filter, config.t_sb);
        ESP_LOGW(TAG, "Over Sampling Temp : %d, Over Sampling Press : %d\r", config.osrs_T, config.osrs_P);
        ESP_LOGW(TAG, "Over Sampling Hum : %d\r", config.osrs_H);
        ESP_LOGW(TAG, "/---------------------------------------------/");
        return WRITING_OK;
    }
    return WRITING_FAIL;
}

/**********************************
 * BME280 return functions
**********************************/

int8_t BME280_Measuring(i2c_sensor_t sensor)
{
    uint8_t Meas_value = 255;

    esp_err_t ret = i2c_read_bit(sensor, STATUS_REG, &Meas_value);

    Meas_value &= 0b00001000;
    Meas_value = Meas_value >> 3;

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Measuring value failed, ERROR Message : %s", esp_err_to_name(ret));
        return -1;
    }

    return (int8_t)Meas_value;
}

/*---------------------------------------------------------------------------------------------------*/

int8_t BME280_Im_Update(i2c_sensor_t sensor)
{
    uint8_t Update_value = 255;

    esp_err_t ret = i2c_read_bit(sensor, STATUS_REG, &Update_value);

    Update_value &= 0b0000001;

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Update read failed, ERROR Message : %s", esp_err_to_name(ret));
        return -1;
    }

    return (int8_t) Update_value;
}

/*---------------------------------------------------------------------------------------------------*/

int8_t BME280_Read_Chip_ID(i2c_sensor_t sensor)
{
    uint8_t ChipID = 255;

    esp_err_t ret = i2c_read_bit(sensor, STATUS_REG, &ChipID);

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Chip ID read failed, ERROR Message : %s", esp_err_to_name(ret));
        return -1;
    }

    ESP_LOGI(TAG, "Chip ID : %d / %X", ChipID, ChipID);
    return (int8_t)ChipID;
}

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_Read_Compensation(i2c_sensor_t sensor)
{
    uint8_t comp_tab[26] = {0};
    uint8_t comp_tab2[7] = {0};

    esp_err_t ret = i2c_read_bits(sensor, CALIB_1_REG, comp_tab, 25);

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Read calibration 1 failed, ERROR message : %s", esp_err_to_name(ret));
        return READING_FAIL;
    }
    CompenParam.dig_T1 = ((uint8_t)comp_tab[1] << 8) + (uint8_t)comp_tab[0];
    CompenParam.dig_T2 = (comp_tab[3] << 8) + comp_tab[2];
    CompenParam.dig_T3 = (comp_tab[5] << 8) + comp_tab[4];
    CompenParam.dig_P1 = ((uint8_t)comp_tab[7] << 8) + (uint8_t)comp_tab[6];
    CompenParam.dig_P2 = (comp_tab[9] << 8) + comp_tab[8];
    CompenParam.dig_P3 = (comp_tab[11] << 8) + comp_tab[10];
    CompenParam.dig_P4 = (comp_tab[13] << 8) + comp_tab[12];
    CompenParam.dig_P5 = (comp_tab[15] << 8) + comp_tab[14];
    CompenParam.dig_P6 = (comp_tab[17] << 8) + comp_tab[16];
    CompenParam.dig_P7 = (comp_tab[19] << 8) + comp_tab[18];
    CompenParam.dig_P8 = (comp_tab[21] << 8) + comp_tab[20];
    CompenParam.dig_P9 = (comp_tab[23] << 8) + comp_tab[22];
    CompenParam.dig_H1 = (comp_tab[24]);

    ret = i2c_read_bits(sensor, CALIB_2_REG, comp_tab2, 7);

    if(ret != ESP_OK)
    {
        ESP_LOGW(TAG, "Read calibration 2 failed, ERROR message : %s", esp_err_to_name(ret));
        return READING_FAIL;
    }
    CompenParam.dig_H2 = (comp_tab2[1] << 8) + comp_tab2[0];
    CompenParam.dig_H3 = comp_tab2[2];
    CompenParam.dig_H4 = (comp_tab2[3] << 4) | (comp_tab2[4] & 0b00001111);
    CompenParam.dig_H5 = ((comp_tab2[4] & 0b11110000) >> 4) | (comp_tab2[5] << 4);
    CompenParam.dig_H6 = comp_tab2[6];
    return READING_OK;
}

/*---------------------------------------------------------------------------------------------------*/

BME280_err_e BME280_Read_T_P_H_values(i2c_sensor_t sensor, int32_t *Raw_values)
{
    uint8_t i2c_values[8] = {0};

    esp_err_t ret = i2c_read_bits(sensor, PRESS_MSB_REG, i2c_values, 8);

    if(ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Sensor measuring values failed, Error message : %s", esp_err_to_name(ret));
        Raw_values[0] = -1;
        Raw_values[1] = -1;
        Raw_values[2] = -1;
        return READING_FAIL;
    }
    Raw_values[0] = (i2c_values[temp_xlsb] >> 4) + (i2c_values[temp_lsb] << 4) + (i2c_values[temp_msb] << 12);
    Raw_values[1] = (i2c_values[press_xlsb] >> 4) + (i2c_values[press_lsb] << 4) + (i2c_values[press_msb] << 12);
    Raw_values[2] = i2c_values[hum_lsb] + (i2c_values[hum_msb] << 8);
    return READING_OK;
}

/*---------------------------------------------------------------------------------------------------*/

double BME280_compensate_T_double(int32_t adc_T)
{
    double var1, var2, T;
    var1 = (((double)adc_T) / 16384.0 - ((double)CompenParam.dig_T1) / 1024.0) * ((double)CompenParam.dig_T2);
    var2 = ((((double)adc_T) / 131072.0 - ((double)CompenParam.dig_T1) / 8192.0) *
            (((double)adc_T) / 131072.0 - ((double)CompenParam.dig_T1) / 8192.0)) * ((double)CompenParam.dig_T3);
    t_fine = (int32_t)(var1 + var2);
    T = (var1 + var2) / 5120.0;
    return T;
}

/*---------------------------------------------------------------------------------------------------*/

double BME280_compensate_P_double(int32_t adc_P)
{
    double var1, var2, p;
    var1 = ((double)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)CompenParam.dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)CompenParam.dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double)CompenParam.dig_P4) * 65536.0);
    var1 = (((double)CompenParam.dig_P3) * var1 * var1 / 524288.0 + ((double)CompenParam.dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)CompenParam.dig_P1);
    if (var1 == 0.0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576.0 - (double)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)CompenParam.dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)CompenParam.dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double)CompenParam.dig_P7)) / 16.0;
    return p;
}

/*---------------------------------------------------------------------------------------------------*/

double BME280_compensate_H_double(int32_t adc_H)
{
    double var_H;
    var_H = (((double)t_fine) - 76800.0);
    var_H = (adc_H - (((double)CompenParam.dig_H4) * 64.0 + ((double)CompenParam.dig_H5) / 16384.0 * var_H)) \
            * (((double)CompenParam.dig_H2) / 65536.0 * (1.0 + ((double)CompenParam.dig_H6) / 67108864.0 * var_H * (1.0 + ((double)CompenParam.dig_H3) / 67108864.0 * var_H)));
    var_H = var_H * (1.0 - ((double)CompenParam.dig_H1) * var_H / 524288.0);
    if (var_H > 100.0)
        var_H = 100.0;
    else if (var_H < 0.0)
        var_H = 0.0;
    return var_H;
}

const char* BME280_err_to_name(BME280_err_e err_code)
{
    switch(err_code)
    {
        case WRITING_OK:
            return "Writing OK";
            break;
        case WRITING_FAIL:
            return "Writing fail";
            break;
        case READING_OK:
            return "Reading OK";
            break;
        case READING_FAIL:
            return "Reading fail";
            break;
        default: 
            return "error missing";
    }
}