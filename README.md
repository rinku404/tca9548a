# TCA9548A I2C Multiplexer Driver

This is an ESP-IDF component for the TCA9548A 1-to-8 I2C multiplexer. It allows you to expand a single I2C bus into 8 separate channels, enabling the use of multiple I2C devices with the same address on the same microcontroller.

## Features

- Compatible with ESP-IDF v5.x (uses `esp_driver_i2c`).
- Supports all 8 channels (0-7).
- Configurable I2C address (0x70 - 0x77).
- Configurable I2C clock speed.
- Uses `heap_caps_calloc` for safe memory allocation.
- Configurable I2C timeout via Kconfig.

## Installation

1. Copy the `tca9548a` folder into the `components` directory of your ESP-IDF project.
2. Add `tca9548a` to the `REQUIRES` list in your main component's `CMakeLists.txt`.

```cmake
idf_component_register(
    SRCS "main.c"
    REQUIRES esp_driver_i2c tca9548a
    INCLUDE_DIRS ""
)
```

## Configuration

This component uses Kconfig for driver behavior settings. Run `idf.py menuconfig` and navigate to **Component config -> TCA9548A Configuration**.

- **I2C Timeout (ms)**: Sets the timeout for I2C transactions. Default is 1000ms. Set to -1 to wait forever.

## Usage

### 1. Initialization

You must first initialize the main I2C master bus using `i2c_new_master_bus`, then pass that handle to the TCA9548A driver.

```c
#include "driver/i2c_master.h"
#include "tca9548a.h"

void app_main(void)
{
    // 1. Initialize Main I2C Bus
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = 0,
        .scl_io_num = 9,  // Adjust pins
        .sda_io_num = 8,  // Adjust pins
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    // 2. Initialize TCA9548A
    tca9548a_config_t tca_conf = {
        .dev_addr = TCA9548A_ADDR_0, // Base address 0x70
        .scl_speed_hz = 400000,      // 400kHz
    };
    
    tca9548a_handle_t tca_handle;
    ESP_ERROR_CHECK(tca9548a_new(bus_handle, &tca_conf, &tca_handle));
}
```

### 2. Selecting a Channel

Once initialized, use `tca9548a_select_channel` to switch the active bus.

```c
// Select Channel 2
ESP_ERROR_CHECK(tca9548a_select_channel(tca_handle, TCA9548A_CHANNEL_2));

// Now, any I2C transactions performed on 'bus_handle' will be routed 
// physically to Channel 2. You can now communicate with sensors 
// connected to that channel.
```

### 3. Deselecting and Cleanup

```c
// Deselect all channels (optional)
tca9548a_select_channel(tca_handle, TCA9548A_CHANNEL_NONE);

// Deinitialize and free memory
tca9548a_del(tca_handle);
```

## API Reference

See `include/tca9548a.h` for full API documentation and configuration options.
