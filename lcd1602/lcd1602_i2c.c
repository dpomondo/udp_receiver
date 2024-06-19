//#include "lcd1602_RGB_Module.h"
#include "pico/stdlib.h"
#include "pico/time.h"
//#include <sys/types.h>
#include "lcd1602_i2c.h"

#define DEBUG           0

static inline void _i2c_write_byte(uint8_t data) {
    int res = i2c_write_blocking(I2C_PORT, 
            LCD_ADDRESS, 
            &data, 
            1, 
            false);
#if DEBUG >= 2
    if (res = PICO_ERROR_GENERIC) {
        printf("Generic Error raised\n");
    } else if (res = PICO_ERROR_TIMEOUT) {
        printf("i2c Timeout Error Raised\n");
    }
#endif // DEBUG 2
}

static inline void _toggle_enable(uint8_t data) {
#define LCD_TOGGLE_DISPLAY 600
    busy_wait_us(LCD_TOGGLE_DISPLAY);
    _i2c_write_byte(data | LCD_ENABLE_PIN);
    busy_wait_us(LCD_TOGGLE_DISPLAY);
    _i2c_write_byte(data & ~LCD_ENABLE_PIN);
    busy_wait_us(LCD_TOGGLE_DISPLAY);
}

static void lcdGenericWrite(uint8_t data, uint8_t rs) {
    uint8_t command = rs | LCD_BACKLIGHT_ON;
    uint8_t high_nibble_plus = (data & 0xF0) | command;
    uint8_t low_nibble_plus = ((data << 4) & 0xF0) | command;

    //_i2c_write_byte(high_nibble_plus);
    _toggle_enable(high_nibble_plus);
    
    //_i2c_write_byte(low_nibble_plus);
    _toggle_enable(low_nibble_plus);
} // lcdGenericWrite

void lcdSetup() {
    /* HD44780 datasheet page 46
     * init by instruction for 4-bit interface
     * */
    sleep_ms(40);
    lcdSendCommand(0x03);
    sleep_ms(5);
    lcdSendCommand(0x03);
    sleep_us(1000);
    lcdSendCommand(0x03);
    sleep_us(1000);
    lcdSendCommand(0x02);

    lcdSendCommand(LCD_FUNCTION_SET | LCD_NUMLINES_2);
    lcdClearScreen();
    lcdSendCommand(LCD_ENTRY_MODE_SET | LCD_CURSOR_INCREMENT);
    lcdSendCommand(LCD_DISPLAY_CTRL | LCD_DISPLAY_ON);
    lcdReturnHome();
}

void lcdSendCommand(uint8_t command) {
    lcdGenericWrite(command, LCD_COMMAND);
}

void lcdPutChar(uint8_t character) {
    lcdGenericWrite(character, LCD_CHARACTER);
}

void lcdPutString(char *string) {
    char c;

    while ( (c = *string) ) {
        lcdPutChar(c);
        string++;
    }
}

void lcdGoToLocation(uint8_t x, uint8_t y) {
    uint8_t command = LCD_DDRAM_ADDR | x;
    if (y > 0) {
        command |= LCD_MEM_WIDTH;
    }
    lcdSendCommand(command);
}
void lcdReturnHome(void) {
    lcdSendCommand(LCD_RETURN_HOME);
}
void lcdClearScreen(void) {
    lcdSendCommand(LCD_CLEAR_DISPLAY);
}

