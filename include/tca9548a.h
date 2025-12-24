#pragma once
#include "esp_err.h"
#include "driver/i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief TCA9548A I2C Address (Configured by pins A0, A1, A2)
 * Base address is 0x70.
 */
typedef enum {
    TCA9548A_ADDR_0 = 0x70, // A2=0, A1=0, A0=0
    TCA9548A_ADDR_1 = 0x71, // A2=0, A1=0, A0=1
    TCA9548A_ADDR_2 = 0x72, // A2=0, A1=1, A0=0
    TCA9548A_ADDR_3 = 0x73, // A2=0, A1=1, A0=1
    TCA9548A_ADDR_4 = 0x74, // A2=1, A1=0, A0=0
    TCA9548A_ADDR_5 = 0x75, // A2=1, A1=0, A0=1
    TCA9548A_ADDR_6 = 0x76, // A2=1, A1=1, A0=0
    TCA9548A_ADDR_7 = 0x77, // A2=1, A1=1, A0=1
} tca9548a_addr_t;

/**
 * @brief TCA9548A Channels
 */
typedef enum {
    TCA9548A_CHANNEL_0 = 0,
    TCA9548A_CHANNEL_1,
    TCA9548A_CHANNEL_2,
    TCA9548A_CHANNEL_3,
    TCA9548A_CHANNEL_4,
    TCA9548A_CHANNEL_5,
    TCA9548A_CHANNEL_6,
    TCA9548A_CHANNEL_7,
    TCA9548A_CHANNEL_NONE = 0xFF, // Used to deselect all channels
} tca9548a_channel_t;

typedef struct tca9548a_dev_t *tca9548a_handle_t;

/**
 * @brief TCA9548A Configuration Structure
 */
typedef struct {
    tca9548a_addr_t dev_addr; /*!< I2C Address of the device */
    uint32_t scl_speed_hz;    /*!< I2C Clock speed in Hz. If 0, defaults to 400kHz */
} tca9548a_config_t;

/**
 * @brief Initialize a new TCA9548A device
 *
 * @param bus_handle I2C master bus handle
 * @param tca_config Configuration structure
 * @param ret_handle Pointer to store the created device handle
 * @return esp_err_t ESP_OK on success
 */
esp_err_t tca9548a_new(i2c_master_bus_handle_t bus_handle, const tca9548a_config_t *tca_config, tca9548a_handle_t *ret_handle);

/**
 * @brief Select a channel on the TCA9548A multiplexer
 *
 * @param tca_handle Handle obtained from tca9548a_new
 * @param channel Channel to select (0-7) or TCA9548A_CHANNEL_NONE
 * @return esp_err_t ESP_OK on success
 */
esp_err_t tca9548a_select_channel(tca9548a_handle_t tca_handle, tca9548a_channel_t channel);

/**
 * @brief Deinitialize the TCA9548A device and free memory
 * 
 * @param tca_handle Handle obtained from tca9548a_new
 * @return esp_err_t ESP_OK on success
 */
esp_err_t tca9548a_del(tca9548a_handle_t tca_handle);

#ifdef __cplusplus
}
#endif
