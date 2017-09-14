#include "ambient.h"
#include "color_utils.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Internal struct to hold all working data for the led_update_ambient_normal function
 */
typedef struct led_update_piano_ambient_data
{
    unsigned int last_color;      /**< The last color used by the led_update_piano_normal function*/
} led_update_piano_ambient_data_t;

typedef struct led_update_piano_gradient_data
{
    unsigned int left_color;
    unsigned int right_color;
} led_update_ambient_gradient_data_t;

void *new_led_update_ambient_normal_data_t()
{
    led_update_piano_ambient_data_t *ret = malloc(sizeof(led_update_piano_ambient_data_t));

    //set everything to 0
    ret->last_color = 0;

    return ret;
}

void *new_led_update_ambient_gradient_data_t()
{
    led_update_ambient_gradient_data_t *ret = malloc(sizeof(led_update_ambient_gradient_data_t));

    //set everything to 0
    ret->left_color = 0;
    ret->right_color = 0;

    return ret;
}

void led_update_ambient_normal(led_update_function_data_t *data)
{
    unsigned int color = ((led_update_piano_ambient_data_t *) data->pattern_data)->last_color;

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
        unsigned int adj_color = random_near_color(color,
                                                   RAND_COLOR_THRESHOLD * 3,
                                                   RAND_COLOR_THRESHOLD * 3,
                                                   RAND_COLOR_THRESHOLD * 3);

        //copy color to led_states[]
        data->led_states[i] = adj_color;
    }

    ((led_update_piano_ambient_data_t *) data->pattern_data)->last_color = color;
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

    printf("left_color = %#08x\tright_color = %#08x\n", left_color, right_color);

    //find slopes and intercepts
    double slope_r = ((double) extract_red(right_color) - (double) extract_red(left_color)) / LED_COUNT;
    double slope_b = ((double) extract_blue(right_color) - (double) extract_blue(left_color)) / LED_COUNT;
    double slope_g = ((double) extract_green(right_color) - (double) extract_green(left_color)) / LED_COUNT;

    double int_r = (double) extract_red(left_color);
    double int_b = (double) extract_blue(left_color);
    double int_g = (double) extract_green(left_color);

    printf("r = i * %f + %f\n", slope_r, int_r);
    printf("b = i * %f + %f\n", slope_b, int_b);
    printf("g = i * %f + %f\n", slope_g, int_g);

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