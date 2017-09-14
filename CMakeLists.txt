project(piano_leds)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-lasound -lpthread -lm")

set(SOURCE_FILES_PIANO
        main_piano.c
        globals.h

        led_patterns/led_patterns.c
        led_patterns/color_utils.c

        rpi_ws281x/dma.c
        rpi_ws281x/mailbox.c
        rpi_ws281x/pcm.c
        rpi_ws281x/pwm.c
        rpi_ws281x/rpihw.c
        rpi_ws281x/ws2811.c

        rpi_ws281x/clk.h
        rpi_ws281x/gpio.h

        pipe/pipe.c)

add_executable(piano_leds ${SOURCE_FILES_PIANO})
