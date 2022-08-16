#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "./avr_common/uart.h"

#define PIN_MASK 0x0F //gli ultimi 4 bit della porta b (50.53)

volatile uint8_t previous_pins;
volatile uint8_t current_pins;

int main(void){
    printf_init();
    DDRB &= ~PIN_MASK; //imposta gli ultimi 4 bit come input
    PORTB |= PIN_MASK; //mette gli ultimi 4 bit in pull up
    while(1){
        current_pins = PINB;
        
        if(current_pins != previous_pins){
            printf("stato cambiato da %x a %x\n",previous_pins,current_pins);
            previous_pins = current_pins;
        }
        _delay_ms(1000);
    }
}