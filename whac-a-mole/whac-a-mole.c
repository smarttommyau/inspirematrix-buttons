#define WS2812BSIMPLE_IMPLEMENTATION
#include "colors.h"
#include "driver.h"
#include "ws2812b_simple.h"

#include <stdio.h>

#define LED_PINS GPIOA, 2


//moles coordinates
const int8_t moles[9][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};



int main(void) {
    SystemInit();
    ADC_init();
    fillgrid();
    WS2812BSimpleSend(LED_PINS, (uint8_t *)led_array, NUM_LEDS * 3);
    // wait for button x
    while (!JOY_X_pressed()) {
        Delay_Ms(10);
    }
    int number_of_moles = 1;
    while (number_of_moles < 10) {
        // clear the board
        clear();
        // generate random number of moles
        for (int i = 0; i < number_of_moles; i++) {
            // generate random position
            int mole = JOY_random() % 9;
            // set the color of the mole
            set_color(mole, (color_t){255, 0, 0});
        }
        // send the data to the led
        WS2812BSimpleSend(LED_PINS, (uint8_t *)led_array, NUM_LEDS * 3);
        // wait for 1 second
        Delay_Ms(1000);
        // clear the board
        clear();
        // send the data to the led
        WS2812BSimpleSend(LED_PINS, (uint8_t *)led_array, NUM_LEDS * 3);
        // wait for 1 second
        Delay_Ms(1000);
        // increase the number of moles
        number_of_moles++;
    }
}
