#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "cgi.h"

// Fill your wifi credentials
const char WIFI_SSID[] = "NETWORK";
const char WIFI_PASSWORD[] = "PASSWORD";

// Global variable storing bit pattern. The size is incremented by the amount of prefix spaces
uint8_t led_pattern[NUM_ROWS][MAX_LENGTH + NUM_MAX7219];
size_t str_length;

int main() {
    stdio_init_all();

    cyw43_arch_init();

    cyw43_arch_enable_sta_mode();

    // Connect to the WiFI network - loop until connected
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
    }
    printf("Connected! \n");

    // Server initialization
    httpd_init();
    printf("Http server initialised\n");

    // Initialize CGI
    cgi_init();
    printf("CGI Handler initialised\n");

    max7219_init();
    
    // Obtain IP address, and generate string to display
    int ip_addr = cyw43_state.netif[CYW43_ITF_STA].ip_addr.addr;
    char ip_string[20];
    sprintf(ip_string, "    My IP: %lu.%lu.%lu.%lu\n", ip_addr & 0xFF, (ip_addr >> 8) & 0xFF, (ip_addr >> 16) & 0xFF, ip_addr >> 24);
    printf("%s\n", ip_string);
    
    // Generate led pattern to display IP address on 8x8 displays
    str_to_led_pattern(ip_string, led_pattern);
    str_length = strlen(ip_string);

    // Infinite loop shifting the pattern. Decrement sleep time to increase speed.
    while (1)
    {
        send_led_pattern(led_pattern);
        shift_led_pattern(led_pattern, str_length);
        sleep_ms(50);
    }
}