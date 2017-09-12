#ifndef PIANO_LEDS_GLOBALS_H
#define PIANO_LEDS_GLOBALS_H

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                21
#define DMA                     5
#define STRIP_TYPE              WS2811_STRIP_GBR        // WS2812/SK6812RGB integrated chip+leds
#define LED_COUNT               88
#define UPDATES_PER_SEC         16

#define DECAY_IF_SUSTAIN        .95
#define DECAY_IF_N_SUSTAIN      .5

#endif //PIANO_LEDS_GLOBALS_H
