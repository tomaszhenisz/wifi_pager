#ifndef MAX7219_H
#define MAX7219_H

#include "hardware/spi.h"
#include "pico/stdlib.h"

#define SPI_PORT spi0
#define PIN_SCK 18
#define PIN_MOSI 19
#define PIN_CS 17
#define BAUD_RATE 1500000 // Adjust as needed

#define DISPLAY_TEST_REG 0xF
#define SCAN_LIMIT_REG 0x0B
#define DECODE_MODE_REG 0x9
#define SHUTDOWN_REG 0xC

#define NUM_MAX7219 8 // Number of MAX7219s ICs in the chain
#define NUM_ROWS 8 // Number of rows of each display, typically ther are 8x8
#define MAX_LENGTH 100 // Max length of displayed string

// Function sending control commands to configure MAX7219
void max7219_send_ctrl_command(uint8_t , uint8_t);

// Function sending data to be displayed
void max7219_send_data_command(uint8_t address, uint8_t data[]);

// Function initializing MAX7219 (initialize SPI, set max intensity, set scan limit to 7, no test mode, no decoding)
void max7219_init();

// Function setting intensity to desired value
void max7219_set_intensity(uint8_t intensity);

// Function clearing artifacts at initial startup
void max7219_clear_display();

// Function to convert a string to an LED pattern
void str_to_led_pattern(const char *str, uint8_t led_pattern[NUM_ROWS][MAX_LENGTH + NUM_MAX7219]);

// Function to send LED pattern data to MAX7219, display row by row
void send_led_pattern(uint8_t led_pattern[NUM_ROWS][MAX_LENGTH + NUM_MAX7219]);

// Function shifting led pattern by 1 pixel
void shift_led_pattern(uint8_t led_pattern[NUM_ROWS][MAX_LENGTH + NUM_MAX7219], size_t width);

// Function adding specified number of spaces at the beginning of the string
// Used to make the string slide into the view
char* prefix_spaces(const char* input_str, int num_spaces);
#endif