idf_component_register(SRCS "tca9548a.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_driver_i2c esp_log esp_common)