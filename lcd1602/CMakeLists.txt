add_library(lcd1602
    lcd1602_i2c.h
    lcd1602_i2c.c
    )

target_link_libraries(lcd1602
        pico_stdlib
        hardware_i2c
        )

target_include_directories(lcd1602 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
