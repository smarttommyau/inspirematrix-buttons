#define WS2812BSIMPLE_IMPLEMENTATION
#include "colors.h"
#include "driver.h"
#include "fonts.h"
#include "ws2812b_simple.h"

#include <stdio.h>

#define LED_PINS GPIOA, 2


//moles coordinates
int8_t molesmap[64] = {0};
int main(void) {
    SystemInit();
    ADC_init();
    WS2812BSimpleSend(LED_PINS, (uint8_t *)led_array, NUM_LEDS * 3);
    // wait for button x
    uint16_t seed = 0;
    while (!JOY_X_pressed()) {
        seed++;
        if (seed >= 65530) {
            seed = 0;
        }
        Delay_Ms(10);
    }
    uint8_t number_of_moles = 1;
    uint8_t points = 0;
    JOY_setseed(seed);
    while (number_of_moles < 10) {
        // clear the board
        clear();
        // generate random number of moles
        for (uint8_t i = 0; i < number_of_moles; i++) {
            // generate random position
            uint8_t mole = JOY_random() % 64;
            while(molesmap[mole] == 1){
                mole = JOY_random() % 64;
            }
            // set the color of the mole
            molesmap[mole] = 1;
            set_color(mole, (color_t){255, 0, 0});
            printf("Mole at %d %d\n", mole/8, mole%8);
        }
        // send the data to the led
        WS2812BSimpleSend(LED_PINS, (uint8_t *)led_array, NUM_LEDS * 3);
        // wait for 2 second and detect touch
        uint8_t position = -1;
        for(uint8_t iter = 0;iter < 200; iter++){
            position = matrix_pressed(ADC_read);
            if(molesmap[position] == 1){
                set_color(position, (color_t){0, 0, 0});
                WS2812BSimpleSend(LED_PINS, (uint8_t *)led_array, NUM_LEDS * 3);
                points++;
            }
            Delay_Ms(10);
        }
        // clear the board
        clear();
        // send the data to the led
        WS2812BSimpleSend(LED_PINS, (uint8_t *)led_array, NUM_LEDS * 3);
        // wait for 1 second
        Delay_Ms(1000);
        // increase the number of moles
        number_of_moles++;
        // clear molesmap and moles
        for (uint8_t i = 0; i < 32; i++) {
            molesmap[i] = 0;
        }
    }
    //show score by looping forever
    //get number of digits
    if(points/10 > 0){
        font_draw(font_list[points/10], colors[8 * points/10 % num_colors], 4);
        font_draw(font_list[points%10], colors[8 * points%10 % num_colors], 0);
    }else{
        font_draw(font_list[points], colors[8 * points % num_colors], 2);
    }
        WS2812BSimpleSend(LED_PINS, (uint8_t *)led_array, NUM_LEDS * 3);
}
