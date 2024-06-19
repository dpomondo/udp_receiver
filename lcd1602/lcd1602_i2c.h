#ifndef _NEW_LCD_I2C_H
#define _NEW_LCD_I2C_H

/* Driving a 16x2 LCD via i2c
 *
 * Much of this was stolen from the Raspberry Pi Foundation
 * code example.
 *
 * */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

/* The setup is this:
 *      Pi Pico --> i2c bridge --> HD44780 controller
 *
 * Level shifting is required to go from the 3.3v pico to the 5-volt LCD.
 *
 * Messages sent have two distinct part:    1. the message to the HD44780
 *                                          2. the structure sent via i2c
 *
 * The HD44780 can take data in either 8-bit or 4-bit mode, over 8
 * data lines DB[7:0] or four data lines DB[7:4], along with several 
 * single-purpose control lines:
 *      RS  --> Register Select (either the Command or Character register)
 *      R/W --> Read/Write (write low -- there is no reading over i2c, only
*               writing so this line is ignored)
 *      E   --> Enable (toggled to tell the HD44780 to read the data lines)
 *      A   --> Backlight(?? or is it VO??)
 *
 * Sending via i2c uses 4-bit mode, with the following structure:
 *
 *             Backlight on/off (0x08)
 *             |   (possibly the read/write bit?)
 *             |   | Register Select bit (0x01)
 *             |   | |
 *  0b x x x x x x x x
 *     | | | |   |  
 *     | | | |   Enable bit (0x04b)
 *     | | | DB4 data line
 *     | | DB5 data line
 *     | DB6 data line
 *     DB7 data line
 *
 * The 8-bit instruction to the HD44780 will be split into two nibbles and then
 * packed into the DB[7:4] bits of the i2c message. Then the enable bit will be
 * toggled by first sending the message with the enable OFF, then the message 
 * with the enable ON, and finally the message with the enable OFF, like so:
 *      i2c(message_nibble & ~ENABLE) --> OFF
 *      i2c(message_nibble | ENABLE)  --> ON
 *      i2c(message_nibble & ~ENABLE) --> OFF
 *      
 * The first half of these defines will include HD44780 message-specific bits
 * -- which will get shifted -- and control bits which will not. This mimics
 *  the physical interface with the HD44780 which includes data lines and
 *  specific control lines.
 *  */

/* Instruction defines
 * see page 24 of HD44780 datasheet
 * NOTE: these are in binary so make explicit the relationship between the bit
 * and the physical data line in a hypothetical hard-wired set-up. Sending
 * a hard-wired <CLear Display> command, for example, would entail toggling 
 * the data line DB0 to high, then toggling the Enable line on and off. 
 * */ 

#define LCD_CLEAR_DISPLAY     0b00000001 
#define LCD_RETURN_HOME       0b00000010
#define LCD_ENTRY_MODE_SET    0b00000100
#define LCD_DISPLAY_CTRL      0b00001000
#define LCD_CUR_DISP_SHIFT    0b00010000
#define LCD_FUNCTION_SET      0b00100000
#define LCD_CGRAM_ADDR        0b01000000 //  address in bits [5:0] 
#define LCD_DDRAM_ADDR        0b10000000 // adress in bits [6:0]

/* LCD_ENTRY_MODE_SET
 * */
#define LCD_CURSOR_INCREMENT  0b00000010 // cursor moves to the right when
                                              // incremented by one.
#define LCD_DISPLAY_SHIFT     0b00000001 // if enabled, the cursor will
                                              // seem to stay atationary and
                                              // the screen will move instead.
                                              
/* LCD_DISPLAY_CTRL
 * */
#define LCD_DISPLAY_ON        0b00000100 // turns on display
#define LCD_CURSOR_ON         0b00000010 // 1: cursor on 0: cursor off
#define LCD_CURSOR_BLINK      0b00000001 // 1: cursor blink 0: no blink

/* LCD_CUR_DISP_SHIFT
 * */
#define LCD_SHIFT_OPTION      0b00001000 // 1: Display shift 0: cursor
                                              // shift
#define LCD_SHIFTRIGHT        0b00000100 // 1: right 0: left
                                              //
/* LCD_FUNCTION_SET
 * */
#define LCD_DATALENGTH_8      0b00010000 // 0: 4-bit data length
                                              // 1: 8-bit data length
#define LCD_NUMLINES_2        0b00001000 // 0: 1-line display
                                              // 1: 2-line display
#define LCD_5BY10DOTS         0b00000100 // 1: 5x10 font
                                              // 0: 5x8 font
                                              // Most common displays only use
                                              // the 5x8 font

/* Control bits
 * */
#define LCD_COMMAND           0b00000000
#define LCD_CHARACTER         0b00000001
#define LCD_ENABLE_PIN        0b00000100
#define LCD_BACKLIGHT_ON      0b00001000

/* i2c info
 * */
/* LCD info
 * */
#define I2C_PORT            i2c0
#ifndef _lcd_dumbass_rbg_
#define LCD_ADDRESS         0x27
#else
#define LCD_ADDRESS     (0x7c>>1)
#define RGB_ADDRESS     (0xc0>>1)
#endif // _lcd_dumbass_rbg_

#define LCD_NUM_LINES       2
#define LCD_SCREEN_WIDTH    16
#define LCD_MEM_WIDTH       0x40

void lcdSetup();
void lcdSendCommand(uint8_t command);
void lcdPutChar(uint8_t character);
void lcdPutString(char *string);
void lcdGoToLocation(uint8_t x, uint8_t y);
void lcdReturnHome(void);
void lcdClearScreen(void);

#endif // NEW_LCD define
