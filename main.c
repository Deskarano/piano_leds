#include <alsa/asoundlib.h>
#include <pthread.h>

#include "led_patterns/led_patterns.h"

typedef struct midi_collector_thread_arg
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

    //init main loop
    void (*led_update_function)(ws2811_t *, pipe_consumer_t *, led_update_function_data *);
    led_update_function = led_update_piano_normal;

    led_update_piano_normal_data *data = new_led_update_piano_normal_data();
    led_update_function_data *data_union = malloc(sizeof(led_update_function_data));
    data_union->piano_normal = data;

    while(1)
    {
        led_update_function(led_string, consumer, data_union);

        if(ws2811_render(led_string) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_init failed");
            pipe_consumer_free(consumer);
            exit(1);
        }

        usleep(1000000 / UPDATES_PER_SEC);
    }
}