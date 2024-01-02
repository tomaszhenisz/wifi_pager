# wifi pager
 
This project implements a server on the Raspberry Pi Pico W, that hosts an SHTML page.
Users can enter a messages in the text box.

A message is processed by CGI to be displayed on an 8x8 LED displays driven by MAX7219 chips.

## Demonstration
https://github.com/tomaszhenisz/wifi_pager/assets/151632431/9af86871-4391-436b-9df5-e27904dae6ed

Video editing by Klaudia Henisz

## Features

- Raspberry Pi Pico W based server
- SHTML page with a text box for user input
- CGI processing to display user-entered messages on the LED display
- Communication with MAX7219 chips using SPI hardware
- Display of the server's IP address on the LED display upon initialization

## Hardware Requirements

- Raspberry Pi Pico W
- 8x8 LED display panel(s) with MAX7219 chips

## Installation and Usage

- Clone the repository. Remember to enter your own wifi credentials in main.c
- Use the following wiring:
 
  PIN22 (SPI0 CSn) ------------> CS
  
  PIN24 (SPI0 SCK) ------------> CLK
  
  PIN25 (SPI0 TX) -------------> DIN
- Go to displayed IP address and enter you message
