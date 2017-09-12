project(piano_leds)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-lasound -lpthread -lm")

set(SOURCE_FILES_PIANO
        main_piano.c
        src/globals.h

        src/led_patterns/led_patterns.c
        src/led_patterns/color_utils.c

        src/rpi_ws281x/dma.c
        src/rpi_ws281x/mailbox.c
        src/rpi_ws281x/pcm.c
        src/rpi_ws281x/pwm.c
        src/rpi_ws281x/rpihw.c
        src/rpi_ws281x/ws2811.c

        src/rpi_ws281x/clk.h
        src/rpi_ws281x/gpio.h

        src/pipe/pipe.c)

add_executable(piano_leds ${SOURCE_FILES_PIANO})

set(SOURCE_FILES_AMBIENT
        main_ambient.c
        src/globals.h

        src/led_patterns/color_utils.h

        src/rpi_ws281x/dma.c
        src/rpi_ws281x/mailbox.c
        src/rpi_ws281x/pcm.c
        src/rpi_ws281x/pwm.c
        src/rpi_ws281x/rpihw.c
        src/rpi_ws281x/ws2811.c

        src/rpi_ws281x/clk.h
        src/rpi_ws281x/gpio.h)

add_executable(ambient_leds ${SOURCE_FILES_AMBIENT})