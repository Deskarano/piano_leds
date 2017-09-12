#include "src/globals.h"
#include "src/rpi_ws281x/ws2811.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    //init LEDs
    ws2811_t *led_string = malloc(sizeof(ws2811_t));

    led_string->freq = TARGET_FREQ;
    led_string->dmanum = DMA;

    led_string->channel[0].gpionum = GPIO_PIN;
    led_string->channel[0].count = LED_COUNT;
    led_string->channel[0].invert = 0;
    led_string->channel[0].brightness = 255;
    led_string->channel[0].strip_type = STRIP_TYPE;

    if(ws2811_init(led_string) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_init failed");
        exit(1);
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        led_string->channel[0].leds[i] = 0;
    }

    printf("currently on branch ambient_leds\n");
}