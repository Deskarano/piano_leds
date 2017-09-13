#include "src/globals.h"
#include "src/rpi_ws281x/ws2811.h"
#include "src/led_patterns/color_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

    srandom((unsigned int) time(NULL));

    unsigned int color = 0;

    while(1)
    {
        if(color == 0)
        {
            color = (unsigned int) random();
        }
        else
        {
            color = random_near_color(color, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD);
        }

        for(int i = 0; i < LED_COUNT; i++)
        {
	    unsigned int adj_color = random_near_color(color, RAND_COLOR_THRESHOLD * 3, RAND_COLOR_THRESHOLD * 3, RAND_COLOR_THRESHOLD * 3);
            led_string->channel[0].leds[i] = adj_color;
        }

        if(ws2811_render(led_string) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed");
            exit(1);
        }

        usleep(1000000 / UPDATES_PER_SEC);
    }
}
