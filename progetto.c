#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "./avr_common/uart.h"

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

int main(){
    printf_init();
    adc_setup();
    while(1){
        volatile unsigned int x = 0;
        static const float ADC_to_V = 1*5/1023.0;
        static const int ADC_CHANNELS = 3;
        volatile float ADC_read[ADC_CHANNELS];
        memset(ADC_read,0,ADC_CHANNELS*sizeof(float));
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
            ADC_read[i] = (float)sum*n_inv*ADC_to_V;
            i++;
            ADMUX +=2;
            sum = 0;
        }
        ADMUX=0;
        ADMUX |= (1<<MUX0);
        ADMUX |= (1<<REFS0);
        printf("%20.18lf,%20.18lf,%20.18lf\n",ADC_read[0],ADC_read[1],ADC_read[2]);
    }
}