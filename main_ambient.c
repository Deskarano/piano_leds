#include "src/globals.h"
#include "src/rpi_ws281x/ws2811.h"
#include "src/led_patterns/led_patterns_ambient.h"

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

    led_update_function_data_t *data = new_led_update_function_data_t();
    data->current_pattern = AMBIENT_NORMAL;
    data->current_update_function = led_update_ambient_normal;
    data->new_current_update_function_data = new_led_update_ambient_normal_data;

    while(1)
    {
        run_led_update_function(data);
        
        for(int i = 0; i < LED_COUNT; i++)
        {
            led_string->channel[0].leds[i] = data->led_states[i];
        }

        if(ws2811_render(led_string) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed");
            exit(1);
        }

        usleep(1000000 / UPDATES_PER_SEC);
    }
}
