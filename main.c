#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#include <alsa/asoundlib.h>

#include "rpi_ws281x/ws2811.h"
#include "pipe/pipe.h"

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                21
#define DMA                     5
#define STRIP_TYPE              WS2811_STRIP_GBR        // WS2812/SK6812RGB integrated chip+leds
#define LED_COUNT               150

#define UPDATES_PER_SEC         60

typedef struct
{
    pipe_producer_t *producer;
    snd_rawmidi_t *midi_in;
} midi_collector_thread_arg;

void *midi_collector_thread(void *arg)
{
    pipe_producer_t *producer = ((midi_collector_thread_arg *) arg)->producer;
    snd_rawmidi_t *midi_in = ((midi_collector_thread_arg *) arg)->midi_in;

    unsigned char buffer[1];

    while(1)
    {
        if(snd_rawmidi_read(midi_in, buffer, 1) < 0)
        {
            fprintf(stderr, "Problem reading MIDI input");
            pipe_producer_free(producer);
            exit(1);
        }

        if(buffer[0] != 254)
        {
            printf("pushing %i\n", buffer[0]);
            pipe_push(producer, buffer, 1);
        }
    }
}

int main()
{
    //init pipes
    pipe_t *pipe = pipe_new(sizeof(unsigned char), 0);

    pipe_producer_t *producer = pipe_producer_new(pipe);
    pipe_consumer_t *consumer = pipe_consumer_new(pipe);

    pipe_free(pipe);

    //init MIDI
    snd_rawmidi_t *midi_in = NULL;

    if(snd_rawmidi_open(&midi_in, NULL, "hw:1,0,0", SND_RAWMIDI_SYNC) < 0)
    {
        fprintf(stderr, "snd_rawmidi_open failed");
        exit(1);
    }

    midi_collector_thread_arg *arg = malloc(sizeof(midi_collector_thread_arg));
    arg->producer = producer;
    arg->midi_in = midi_in;

    pthread_t *midi_thread_handle = malloc(sizeof(pthread_t));
    pthread_create(midi_thread_handle, NULL, midi_collector_thread, arg);

    //Initiate LEDs
    ws2811_t led_string;

    led_string.freq = TARGET_FREQ;
    led_string.dmanum = DMA;

    led_string.channel[0].gpionum = GPIO_PIN;
    led_string.channel[0].count = LED_COUNT;
    led_string.channel[0].invert = 0;
    led_string.channel[0].brightness = 255;
    led_string.channel[0].strip_type = STRIP_TYPE;

    if(ws2811_init(&led_string) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_init failed");
        exit(1);
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        led_string.channel[0].leds[i] = 0;
    }

    int keys_pressed[LED_COUNT];
    int keys_to_sustain[LED_COUNT];

    for(int i = 0; i < LED_COUNT; i++)
    {
        keys_pressed[i] = 0;
        keys_to_sustain[i] = 0;
    }

    unsigned char buffer[2];

    while(1)
    {
        if(pipe_size((pipe_generic_t *) consumer) > 0)
        {
            ws2811_led_t color = (uint32_t) (random() % 0xFFFFFF);

            while(pipe_size((pipe_generic_t *) consumer) > 0)
            {
                pipe_pop(consumer, buffer, 1);

                if(buffer[0] == 144)
                {
                    pipe_pop(consumer, buffer, 2);

                    if(buffer[1] > 0)
                    {
                        led_string.channel[0].leds[buffer[0] - 21] = color;
                        keys_pressed[buffer[0] - 21] = 1;
                    }
                    else
                    {
                        keys_pressed[buffer[0] - 21] = 0;
                    }
                }

                if(buffer[0] == 176)
                {
                    pipe_pop(consumer, buffer, 2);

                    if(buffer[1] > 0)
                    {
                        for(int i = 0; i < LED_COUNT; i++)
                        {
                            if(keys_pressed[i]) keys_to_sustain[i] = 1;
                        }
                    }
                    else
                    {
                        for(int i = 0; i < LED_COUNT; i++)
                        {
                            if(!keys_pressed[i]) keys_to_sustain[i] = 0;
                        }
                    }
                }
            }
        }

        for(int i = 0; i < LED_COUNT; i++)
        {
            if(!(keys_to_sustain[i]))
            {
                led_string.channel[0].leds[i] = 0;
            }
        }

        if(ws2811_render(&led_string) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_init failed");
            pipe_consumer_free(consumer);
            exit(1);
        }

        usleep(1000000 / UPDATES_PER_SEC);
    }
}
