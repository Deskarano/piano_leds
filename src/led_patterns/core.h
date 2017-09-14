#ifndef SMART_LEDS_LED_PATTERNS_CORE_H
#define SMART_LEDS_LED_PATTERNS_CORE_H

#include "../globals.h"

/**
 * Defined LED patterns
 */
typedef enum PATTERN
{
    NONE,
    PIANO_NORMAL,
    PIANO_WAR,
    AMBIENT_NORMAL
} PATTERN;

/**
 * Custom type for functions that act as LED update functions
 */
typedef void (*led_pattern_function)(struct led_update_function_data *);

/**
 * Custom type for functions that create LED update function data
 */
typedef void *(*new_led_pattern_function_data)();

/**
 * Struct to hold all necessary information to run the LED strip on a certain mode
 *
 * @param current_pattern The current LED pattern being used
 * @param last_pattern The LED pattern being run in the last cycle
 * @param led_pattern_function The current LED update function being used
 * @param new_led_pattern_function_data The current LED update function data generator
 * @param pattern_data Pointer to an LED pattern-specific data struct
 * @param led_states[] The array containing current LED states to be written to the strip
 * @param buffer[2] A buffer used for internal operations
 * @param data_pipe A pointer to a communications related struct, if needed
 */
typedef struct led_update_function_data
{
    PATTERN current_pattern;
    PATTERN last_pattern;

    led_pattern_function current_update_function;
    new_led_pattern_function_data new_current_update_function_data;

    void *pattern_data;

    unsigned int led_states[LED_COUNT];
    unsigned char buffer[2];

    void *data_pipe;
} led_update_function_data_t;

/**
 * Function which generates and initializes new led_update_function_data_t structs
 * @return
 */
led_update_function_data_t *new_led_update_function_data_t();

/**
 * Function which takes an led_update_function_data_t and runs the LED pattern specified by it
 */
void run_led_update_function(led_update_function_data_t *);

#endif //SMART_LEDS_LED_PATTERNS_CORE_H
