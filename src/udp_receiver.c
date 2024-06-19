#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "lcd1602/lcd1602_i2c.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

int main()
{

    stdio_init_all();

      

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    lcdSetup();
    sleep_ms(40);       // let the LCD finish setting up

    char greeting[] = "10hELLO wORLD!";
    
    // uint8_t x = (uint8_t)greeting[0] - 48;
    uint8_t x = greeting[0] - '0';
    // uint8_t y = (uint8_t)greeting[1] - 48;
    uint8_t y = greeting[1] - '0';
    printf("%s at %d, %d\r\n", greeting, x, y);
    lcdGoToLocation(x,y);
    lcdPutString(&greeting[2]);

    return 0;
}
