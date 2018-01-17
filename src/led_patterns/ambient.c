#include "ambient.h"
#include "color_utils.h"

#include "../globals.h"

#include <stdlib.h>

/**
 * Internal struct to hold all working data for the led_update_ambient_normal function
 */
typedef struct led_update_piano_ambient_data
{
    unsigned int last_color;      /**< The last color used by the led_update_piano_normal function*/
    double *factors;              /**< Some random factors to change the color by */
} led_update_ambient_normal_data_t;

typedef struct led_update_piano_gradient_data
{
    unsigned int left_color;    /**< The color to the left of the gradient */
    unsigned int right_color;   /**< The color to the right of the gradient */
} led_update_ambient_gradient_data_t;

void *new_led_update_ambient_normal_data_t()
{
    //allocate memory
    led_update_ambient_normal_data_t *ret = malloc(sizeof(led_update_ambient_normal_data_t));
    ret->factors = malloc(LED_COUNT * sizeof(double));

    //set everything to 0
    ret->last_color = 0;

    for(int i = 0; i < LED_COUNT; i++)
    {
        ret->factors[i] = ((double) (random() % 50) + 75) / 100;
    }

    return ret;
}

void free_led_update_ambient_normal_data_t(void *data)
{
    free(((led_update_ambient_normal_data_t *) data)->factors);
    free(data);
}

void *new_led_update_ambient_gradient_data_t()
{
    led_update_ambient_gradient_data_t *ret = malloc(sizeof(led_update_ambient_gradient_data_t));

    //set everything to 0
    ret->left_color = 0;
    ret->right_color = 0;

    return ret;
}

void free_led_update_ambient_gradient_data_t(void *data)
{
    free(data);
}

void led_update_ambient_normal(led_update_function_data_t *data)
{
    unsigned int color = ((led_update_ambient_normal_data_t *) data->pattern_data)->last_color;
    double *factors = ((led_update_ambient_normal_data_t *) data->pattern_data)->factors;

    if(color == 0)
    {
        //generate a new color
        color = (unsigned int) random();
    }
    else
    {
        //get a close color
        color = random_near_color(color,
                                  RAND_COLOR_THRESHOLD,
                                  RAND_COLOR_THRESHOLD,
                                  RAND_COLOR_THRESHOLD);
    }

    for(int i = 0; i < LED_COUNT; i++)
    {
        //introduce some randomness
        unsigned int adj_color = adjacent_color(color, factors[i]);

        //copy color to led_states[]
        data->led_states[i] = adj_color;
    }

    ((led_update_ambient_normal_data_t *) data->pattern_data)->last_color = color;
}

void led_update_ambient_gradient(led_update_function_data_t *data)
{
    //store colors in local variables so less typing
    unsigned int left_color = ((led_update_ambient_gradient_data_t *) data->pattern_data)->left_color;
    unsigned int right_color = ((led_update_ambient_gradient_data_t *) data->pattern_data)->right_color;

    //generate new colors or modify the given ones
    if(left_color == 0)
    {
        left_color = (unsigned int) random() % 0xFFFFFF;
    }
    else
    {
        left_color = random_near_color(left_color, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD);
    }

    if(right_color == 0)
    {
        right_color = (unsigned int) random() % 0xFFFFFF;
    }
    else
    {
        right_color = random_near_color(right_color, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD, RAND_COLOR_THRESHOLD);
    }

    //find slopes and intercepts
    double slope_r = ((double) extract_red(right_color) - (double) extract_red(left_color)) / LED_COUNT;
    double slope_b = ((double) extract_blue(right_color) - (double) extract_blue(left_color)) / LED_COUNT;
    double slope_g = ((double) extract_green(right_color) - (double) extract_green(left_color)) / LED_COUNT;

    unsigned char int_r = extract_red(left_color);
    unsigned char int_b = extract_blue(left_color);
    unsigned char int_g = extract_green(left_color);

    //create a smooth gradient
    for(int i = 0; i < LED_COUNT; i++)
    {
        data->led_states[i] = color_from_channels((unsigned char) (int_r + i * slope_r),
                                                  (unsigned char) (int_b + i * slope_b),
                                                  (unsigned char) (int_g + i * slope_g));
    }

    ((led_update_ambient_gradient_data_t *) data->pattern_data)->left_color = left_color;
    ((led_update_ambient_gradient_data_t *) data->pattern_data)->right_color = right_color;
}