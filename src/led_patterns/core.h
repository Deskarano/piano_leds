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
} Pattern;

/**
 * Behind the scenes struct for led_update_function_data_t
 */
struct led_update_function_data;

/**
 * Custom type for functions that act as LED update functions
 */
typedef void (*led_pattern_function)(struct led_update_function_data *);

/**
 * Struct to hold all necessary information to run the LED strip on a certain mode
 */
typedef struct led_update_function_data
{
    Pattern current_pattern;
    Pattern last_pattern;

    led_pattern_function current_update_function;   /**< The current LED update function */

    void *pattern_data;                             /**< Pointer to LED update function specific data */

    unsigned int led_states[LED_COUNT];             /**< At the end of every iteration, contains states of LEDs */
    unsigned char buffer[2];                        /**< The current LED pattern to run */

    void *data_pipe;                                /**< An optional data type for update function communication */
} led_update_function_data_t;

/**
 * Function which generates and initializes new led_update_function_data_t structs
 * @return
 */
led_update_function_data_t *new_led_update_function_data_t();

/**
 * Function which takes an led_update_function_data_t and runs the LED pattern specified by it
 *
 * @param data The data for the LED pattern to run
 */
void run_led_update_function(led_update_function_data_t *data);

#endif //SMART_LEDS_LED_PATTERNS_CORE_H