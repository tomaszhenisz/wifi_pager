#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include "max7219.h"

extern uint8_t led_pattern[NUM_ROWS][MAX_LENGTH + NUM_MAX7219];
extern size_t str_length;

// Function decoding URL received by CGI (replaces '+' with ' ' etc.)
void url_decode(char *dst, const char *src)
{
    char a, b;
    while (*src)
    {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b)))
        {
            if (a >= 'a')
                a -= 'a' - 'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a' - 'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst++ = 16 * a + b;
            src += 3;
        }
        else if (*src == '+')
        {
            *dst++ = ' ';
            src++;
        }
        else
        {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}

// CGI handler which is run when a request for /print_text.cgi is detected
const char *cgi_print_text_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Check if a request for text has been made (/print_text.cgi?text=some_text)
    if (strcmp(pcParam[0], "text") == 0)
    {
        char *decoded_text = malloc(strlen(pcValue[0]) + 1); // +1 for the null terminator

        if (decoded_text != NULL)
        {
            url_decode(decoded_text, pcValue[0]);

            char *output_str = prefix_spaces(decoded_text, NUM_MAX7219);
            str_to_led_pattern(output_str, led_pattern);

            str_length = strlen(output_str);
            free(decoded_text);
            free(output_str);
        }
    }

    // Send the index page back to the user
    return "/index.shtml";
}

// CGI hander
static const tCGI cgi_handlers[] = {
    {// Html request for "/print_text.cgi" triggers cgi_print_text_handler
     "/print_text.cgi", cgi_print_text_handler},
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 1);
}