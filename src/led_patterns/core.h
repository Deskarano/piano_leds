#ifndef SMART_LEDS_LED_PATTERNS_CORE_H
#define SMART_LEDS_LED_PATTERNS_CORE_H

/**
 * Defined LED patterns
 */
typedef enum PATTERN
{
    NONE,
    PIANO_NORMAL,
    PIANO_WAR,
    AMBIENT_NORMAL,
    AMBIENT_GRADIENT
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
    Pattern current_pattern;                         /**< The current LED pattern to run */
    Pattern last_pattern;                            /**< The last LED pattern that was run, to detect when the
 *                                                        pattern changes*/

    led_pattern_function current_update_function;    /**< The current LED update function */

    void *pattern_data;                              /**< Pointer to LED update function specific data */

    unsigned int *led_states;                        /**< At the end of every iteration, contains states of LEDs */
    unsigned char *buffer;

    void *data_pipe;                                 /**< An optional data type for update function communication */
} led_update_function_data_t;

/**
 * Function which generates and initializes new led_update_function_data_t structs
 * @return A pointer to the new led_update_function_data_t struct
 */
led_update_function_data_t *new_led_update_function_data_t();

/**
 * Function which frees all memory used by an led_update_function_data_t
 * @param data A pointer to the led_update_function_data_t to free
 */
void free_led_update_function_data_t(led_update_function_data_t *data);

/**
 * Function which takes an led_update_function_data_t and runs the LED pattern specified by it
 *
 * @param data The data for the LED pattern to run
 */
void run_led_update_function(led_update_function_data_t *data);

#endif //SMART_LEDS_LED_PATTERNS_CORE_H