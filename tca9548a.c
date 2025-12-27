#include <stdio.h>
#include "sdkconfig.h"
#include "tca9548a.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_heap_caps.h"

static const char *TAG = "TCA9548A";

struct tca9548a_dev_t {
    i2c_master_dev_handle_t i2c_dev;
    tca9548a_addr_t address;
};

esp_err_t tca9548a_new(i2c_master_bus_handle_t bus_handle, const tca9548a_config_t *tca_config, tca9548a_handle_t *ret_handle)
{
    ESP_RETURN_ON_FALSE(bus_handle, ESP_ERR_INVALID_ARG, TAG, "Invalid bus handle");
    ESP_RETURN_ON_FALSE(tca_config, ESP_ERR_INVALID_ARG, TAG, "Invalid config");
    ESP_RETURN_ON_FALSE(ret_handle, ESP_ERR_INVALID_ARG, TAG, "Invalid return handle");

    tca9548a_handle_t handle = (tca9548a_handle_t)heap_caps_calloc(1, sizeof(struct tca9548a_dev_t), MALLOC_CAP_DEFAULT);
    ESP_RETURN_ON_FALSE(handle, ESP_ERR_NO_MEM, TAG, "Memory allocation failed");

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = tca_config->dev_addr,
        .scl_speed_hz = tca_config->scl_speed_hz ? tca_config->scl_speed_hz : 400000,
    };

    esp_err_t ret = i2c_master_bus_add_device(bus_handle, &dev_cfg, &handle->i2c_dev);
    if (ret != ESP_OK) {
        heap_caps_free(handle);
        return ret;
    }
    handle->address = tca_config->dev_addr;
    *ret_handle = handle;

    return ESP_OK;
}

esp_err_t tca9548a_select_channel(tca9548a_handle_t tca_handle, tca9548a_channel_t channel)
{
    ESP_RETURN_ON_FALSE(tca_handle, ESP_ERR_INVALID_ARG, TAG, "Invalid handle");
    ESP_RETURN_ON_FALSE((channel <= TCA9548A_CHANNEL_7) || (channel == TCA9548A_CHANNEL_NONE), ESP_ERR_INVALID_ARG, TAG, "Invalid channel number");

    uint8_t data = 0;
    if (channel != TCA9548A_CHANNEL_NONE) {
        data = (1 << channel);
    }
    return i2c_master_transmit(tca_handle->i2c_dev, &data, 1, CONFIG_TCA9548A_I2C_TIMEOUT_MS);
}

esp_err_t tca9548a_del(tca9548a_handle_t tca_handle)
{
    ESP_RETURN_ON_FALSE(tca_handle, ESP_ERR_INVALID_ARG, TAG, "Invalid handle");

    // Remove device from I2C master bus and check for errors
    ESP_RETURN_ON_ERROR(i2c_master_bus_rm_device(tca_handle->i2c_dev), TAG, "Failed to remove I2C device");

    heap_caps_free(tca_handle);
    return ESP_OK;
}
