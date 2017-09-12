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

    unsigned int left_color = (unsigned int) random();
    unsigned int right_color = (unsigned int) random();

    unsigned int colors[LED_COUNT];

    while(1)
    {
        double slope_red = (right_color & 0xFF) - (left_color & 0xFF);
        double slope_blue = ((right_color >> 8) & 0xFF) - ((left_color >> 8) & 0xFF);
        double slope_green = ((right_color >> 16) & 0xFF) - ((left_color >> 16) & 0xFF);

        double int_red = left_color & 0xFF;
        double int_blue = (left_color >> 8) & 0xFF;
        double int_green = (left_color >> 16) & 0xFF;

        for(int i = 0; i < LED_COUNT; i++)
        {
            colors[i] = 0;
            colors[i] += (unsigned int) (slope_red * i + int_red);
            colors[i] += ((unsigned int) (slope_blue * i + int_blue)) << 8;
            colors[i] += ((unsigned int) (slope_green * i + int_green)) << 16;
        }

        for(int i = 0; i < LED_COUNT; i++)
        {
            led_string->channel[0].leds[i] = colors[i];
        }

        if(ws2811_render(led_string) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed");
            exit(1);
        }

        left_color = random_near_color(left_color, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD);
        right_color = random_near_color(right_color, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD);

        usleep(1000000 / UPDATES_PER_SEC);
    }
}