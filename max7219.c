#include "max7219.h"
#include "font8x8_reversed.h"
#include "string.h"
#include "stdlib.h"

void max7219_send_ctrl_command(uint8_t address, uint8_t data)
{
    uint8_t command[] = {address, data};
    // Setting CS active to start transmission
    gpio_put(PIN_CS, 0); 
    for (int i = 0; i < NUM_MAX7219; ++i)
    {
        // Sending 2 byte command for each MAX7219 chip
        spi_write_blocking(SPI_PORT, command, 2);
    }
    // Setting CS inactive
    gpio_put(PIN_CS, 1);
}

void max7219_send_data_command(uint8_t address, uint8_t data[])
{
    // Setting CS active to start transmission
    gpio_put(PIN_CS, 0);
    for (int i = 0; i < NUM_MAX7219; ++i)
    {
        uint8_t command[] = {address, data[i]};
        spi_write_blocking(SPI_PORT, command, 2);
    }
    // Setting CS inactive
    gpio_put(PIN_CS, 1);
}

void max7219_init()
{ // Initialize SPI
    spi_init(SPI_PORT, BAUD_RATE);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    // Set CS to inactive
    gpio_put(PIN_CS, 1);

    // Set necessary registers during initialization
    max7219_send_ctrl_command(SHUTDOWN_REG, 0x01);     // Shutdown Register: 1 (normal operation)
    max7219_send_ctrl_command(DISPLAY_TEST_REG, 0x00); // Display Test Register: 0 (normal operation)
    max7219_send_ctrl_command(SCAN_LIMIT_REG, 0x07);   // Scan Limit Register: 7 (display digits 0-7)
    max7219_send_ctrl_command(DECODE_MODE_REG, 0x00);  // Decode Mode Register: 0 (no decoding)
}

void max7219_set_intensity(uint8_t intensity)
{
    max7219_send_ctrl_command(0x0A, intensity);
}

void max7219_clear_display()
{
    for (int i = 0; i < NUM_MAX7219; ++i)
    {
        for (uint8_t row = 1; row <= 8; ++row)
        {
            max7219_send_ctrl_command(row, 0x00);
        }
    }
}

// Function to convert a string to an LED pattern
void str_to_led_pattern(const char *str, uint8_t led_pattern[NUM_ROWS][MAX_LENGTH + NUM_MAX7219])
{
    // Iterate through each character in the string
    for (int charIndex = 0; str[charIndex] != '\0'; ++charIndex)
    {
        char c = str[charIndex];
        int ascii_value = (int)c;

        // Check if the ASCII value is within the range of the font array
        if (ascii_value >= 0 && ascii_value < 128)
        {
            // Populate the LED pattern array row by row
            for (int row = 0; row < NUM_ROWS; ++row)
            {
                led_pattern[row][charIndex] = font8x8_reversed[ascii_value][row];
            }
        }
        else
        {
            // In case ASCII value is out of range send all 1s (all leds on)
            for (int row = 0; row < NUM_ROWS; ++row)
            {
                led_pattern[row][charIndex] = UINT8_MAX;
            }
        }
    }
}

void send_led_pattern(uint8_t led_pattern[NUM_ROWS][MAX_LENGTH + NUM_MAX7219])
{
    // Iterate through each row of the LED pattern
    for (int row = 0; row < NUM_ROWS; ++row)
    {
        // Iterate through each MAX7219 display
        for (int display = 0; display < NUM_MAX7219; ++display)
        {
            // Send the row data to the MAX7219 display
            max7219_send_data_command(row + 1, led_pattern[row]);
        }
    }
}

void shift_led_pattern(uint8_t led_pattern[NUM_ROWS][MAX_LENGTH + NUM_MAX7219], size_t width)
{
    // Iterate through each row of the LED pattern
    for (int row = 0; row < NUM_ROWS; ++row)
    {
        // Store the right-most bit in tmp
        uint8_t tmp = (led_pattern[row][0] & 0b10000000) >> 7;

        // Iterate through each MAX7219 display
        for (int display = 0; display < width-1; ++display)
        {
            // Assign shifted value, and copy RMB from neighbouring display
            led_pattern[row][display] = (led_pattern[row][display] << 1) | ((led_pattern[row][display + 1] & 0b10000000) >> 7);
        }

        // Add tmp to the last display if width is greater than 1
        if (width > 1)
        {
            // for the last display, copy tmp to RMB
            led_pattern[row][width-1] = (led_pattern[row][width-1] << 1) | tmp;
        }
    }
}

char* prefix_spaces(const char* input_str, int num_spaces) {
    // Allocate memory for the new string with added spaces
    char* output_str = (char*)malloc(strlen(input_str) + num_spaces + 1);

    // Add spaces in front
    for (int i = 0; i < num_spaces; ++i) {
        output_str[i] = ' ';
    }

    // Copy the input string
    strcpy(output_str + num_spaces, input_str);

    return output_str;
}