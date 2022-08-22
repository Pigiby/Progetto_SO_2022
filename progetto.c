#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "./avr_common/uart.h"

#define TCCRA_1_MASK (1<<WGM10)|(1<<COM1C0)|(1<<COM1C1)|(1<<COM1A0)|(1<<COM1A1)
#define TCCRB_1_MASK ((1<<WGM12)|(1<<CS10))
#define TCCRA_3_MASK (1<<WGM30)|(1<<COM3B0)|(1<<COM3B1)
#define TCCRB_3_MASK ((1<<WGM32)|(1<<CS30)|(1<<CS31))
#define TCCRA_4_MASK (1<<WGM40)|(1<<COM4B0)|(1<<COM4B1)
#define TCCRB_4_MASK ((1<<WGM42)|(1<<CS42))
volatile uint8_t interrupt_occurred=0;

// our interrupt routine installed in
// interrupt vector position
// corresponding to output compare
// of timer 5

uint16_t int_count=0;
ISR(TIMER5_COMPA_vect) {
  interrupt_occurred=1;
  int_count++;
}
// adc setup

void adc_setup(){
    //per prima cosa dobbiamo disabilitare gli interrupts
    cli();
    ADMUX=0;
    ADMUX |= (1<<MUX0);
    ADMUX |= (1<<REFS0);
    ADCSRA = 0; //resettiamo adcsra
    ADCSRA |= (1 << ADEN); // attiva adc
    ADCSRA |= (1 << ADPS0) | (1<<ADPS1); //prescaler di 8
    sei();
}
void task(){
    volatile unsigned int x = 0;
        static const float ADC_to_V = 1*5/1023.0;
        static const int ADC_CHANNELS = 3;
        float ADC_read[3] = {0,0,0};
        int i = 0;
        const int n = 50;
        const float n_inv = 1.0/n;
        float sum = 0;
        //iniziamo la conversione
        while(i<3){
            for (int k = 0; k < n;k++){
                ADCSRA |= (1 << ADSC);
                while(ADCSRA & (1 << ADSC)) x++;
                sum += ADC;
            }
            ADC_read[i] = (float)sum*n_inv*ADC_to_V-2.5;
            i++;
            ADMUX=0;
            if(i==1) {
                ADMUX |= (1 << MUX0) | (1 << MUX1) | (1<< REFS0);
            }
            else if(i==2) {
                ADMUX |= (1 << MUX0) | (1 << MUX2) | (1<< REFS0);
                }
            sum = 0;
        }
        printf("%f %f %f %f\n ",int_count/10.0,ADC_read[0],ADC_read[1],ADC_read[2]);
        _delay_ms(100);
        ADMUX=0;
        ADMUX |= (1<<MUX0);
        ADMUX |= (1<<REFS0);
}
int main(){
    TCCR5A = 0;
    TCCR5B = (1 << WGM52) | (1 << CS50) | (1 << CS52); 
    const int timer_duration_ms =100;
    uint16_t ocrval=(uint16_t)(15.62*timer_duration_ms);
    // clear int
     cli();
    TIMSK5 |= (1 << OCIE5A);  // enable the timer interrupt
    // enable int
    sei();
    OCR5A = ocrval;
    printf_init();
    TCCR1A=TCCRA_1_MASK;
    TCCR1B=TCCRB_1_MASK;
    TCCR3A=TCCRA_3_MASK;
    TCCR3B=TCCRB_3_MASK;
    TCCR4A=TCCRA_4_MASK;
    TCCR4B=TCCRB_4_MASK;
    // clear all higher bits of output compare for timer
    OCR1AH=0;
    OCR1BH=0;
    OCR1CH=0;
    OCR1CL=1;
    OCR4AH=0;
    OCR4BH=0;
    OCR4CH=0;
    OCR4BL=1;
    OCR3AH= 0;
    OCR3BH= 0;
    OCR3BL= 1;

  // the LED is connected to pin 13
  // that is the bit 7 of port b, we set it as output
  const uint8_t mask_porta_b=(1<<7);
  //PIN 13 
  const uint8_t mask_porta_e=(1<<4);
  //PIN 2
  const uint8_t mask_porta_h=(1<<4);
  //PIN 7
  // we configure the pin as output
  DDRB |= mask_porta_b;//mask;
  DDRE |= mask_porta_e;
  DDRH |= mask_porta_e;
  uint8_t intensity=0;
    
    adc_setup();
    while(1){
        while (! interrupt_occurred);
        interrupt_occurred=0;
        OCR1CL = intensity;
        OCR4BL = intensity;
        OCR3BL = intensity;
         // from delay.h, wait 1 sec
        intensity+=8;
        task();
        if(int_count >600) return 0;
    }
}