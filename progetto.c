#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "./avr_common/uart.h"

//setting timer per i pin da campionare in modo da cambiarne il valore (utilizzo pwm)
#define TCCRA_1_MASK (1<<WGM10)|(1<<COM1C0)|(1<<COM1C1) 
#define TCCRB_1_MASK ((1<<WGM12)|(1<<CS10)) 
#define TCCRA_3_MASK (1<<WGM30)|(1<<COM3B0)|(1<<COM3B1)
#define TCCRB_3_MASK ((1<<WGM32)|(1<<CS30)|(1<<CS31))
#define TCCRA_4_MASK (1<<WGM40)|(1<<COM4B0)|(1<<COM4B1)
#define TCCRB_4_MASK ((1<<WGM42)|(1<<CS42))
//ipostazioni uart prese dal codice delle esercitazioni
#define BAUD 19200
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUF 256
//variabili volatile utilizzate nell'ISR per inviare i dati al pc
volatile uint8_t interrupt_occurred=0;
volatile int idx = 0;
volatile char ok = 0;
char buf[MAX_BUF];
float f = 0;
ISR(USART0_UDRE_vect) {
  if(buf[idx]){
    UDR0 = buf[idx];
    idx++;
    UCSR0B |= (1<<5);
  }
  else{
    ok = 1;
    UCSR0B &= ~(1<<5);
    idx = 0;
  }
}
void UART_init(void){
  // Set baud rate
  UBRR0H = (uint8_t)(MYUBRR>>8);
  UBRR0L = (uint8_t)MYUBRR;

  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); /* 8-bit data */ 
  UCSR0B = (1<<RXEN0) | (1<<TXEN0);   /* Enable RX and TX */  

}

void UART_putChar(char c){
  // wait for transmission completed, looping on status bit
  while ( !(UCSR0A & (1<<UDRE0)) );

  // Start transmission
  UDR0 = c;
}

char UART_getChar(void){
  // Wait for incoming data, looping on status bit
  while ( !(UCSR0A & (1<<RXC0)) );
  
  // Return the data
  return UDR0;
    
}

// reads a string until the first newline or 0
// returns the size read
char UART_getString(char* buf){
  char* b0=buf; //beginning of buffer
  while(1){
    char c=UART_getChar();
    *buf=c;
    ++buf;
    // reading a 0 terminates the string
    if (c==0)
      return buf-b0;
    // reading a \n  or a \r return results
    // in forcedly terminating the string
    if(c=='\n'||c=='\r'){
      *buf=0;
      ++buf;
      return buf-b0;
    }
  }
}

void UART_putString(char* buf){
  while(*buf){
    UART_putChar(*buf);
    ++buf;
  }
}

// our interrupt routine installed in
// interrupt vector position
// corresponding to output compare
// of timer 5

float int_count=0.0;
ISR(TIMER5_COMPA_vect) {
  interrupt_occurred=1;
  int_count++;
}
// adc setup

void ADC_init(){
    //per prima cosa dobbiamo disabilitare gli interrupts
    cli();
    //Azzeriamo il valore del registro
    ADMUX=0;
    ADMUX |= (1<<MUX0); //selezionato il primo canale per la conversione
    ADMUX |= (1<<REFS0); //refs0 acceso perchè specifichiamo il voltaggio di riferimento
    ADCSRA = 0; //resettiamo adcsra
    ADCSRA |= (1 << ADEN); // attiva adc
    ADCSRA |= (1 << ADPS0) | (1<<ADPS1); //prescaler di 8
    sei();
}
void oscillscope(){
    volatile unsigned int x = 0;
        static const float ADC_to_V = 1*5/1023.0;
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
      
        dtostrf(int_count*f/1000.0, 5, 5, buf);
        strcat(buf," ");
        UCSR0B |= (1<<5);
        while(ok != 1){}
        ok = 0;
        dtostrf(ADC_read[0], 5,5, buf);
        strcat(buf," ");
        UCSR0B |= (1<<5);
        while(ok != 1){}
        ok = 0;
        dtostrf(ADC_read[1], 5, 5, buf);
        strcat(buf," ");
        UCSR0B |= (1<<5);
        while(ok != 1){}
        ok = 0;
        dtostrf(ADC_read[2], 5, 5, buf);
        strcat(buf,"\n");
        UCSR0B |= (1<<5);
        while(ok != 1){}
        ok = 0;
        ADMUX=0;
        ADMUX |= (1<<MUX0);
        ADMUX |= (1<<REFS0);
}
int main(){
    UART_init();
    //polling sul registro rxc0 in attesa della frequenza di campionamento
    //codice delle esercitazioni relative alla parte sulle UART
    char buf[MAX_BUF];
    UART_getString(buf);
    //Convertire la stringa ricevuta in float
    f = atof(buf);
    if(f>32766){
      f=32766;
    }
    //setting del timer 5, timer che serve per campionare i 3 canali
    // wave generator e setting del prescaler
    TCCR5A = 0;
    TCCR5B = (1 << WGM52) | (1 << CS50) | (1 << CS52);
    uint16_t timer_duration_ms=f;
    uint16_t ocrval=(uint16_t)(15.62*timer_duration_ms);
     OCR5A = ocrval;
    cli();
  TIMSK5 |= (1 << OCIE5A);  // enable the timer interrupt
  // enable int
  sei();
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

  const uint8_t mask_porta_b=(1<<7);
  //PIN 13 
  const uint8_t mask_porta_e=(1<<4);
  //PIN 2
  const uint8_t mask_porta_h=(1<<4);
  //PIN 7
  // we configure the pin as output
  DDRB |= mask_porta_b;//mask;
  DDRE |= mask_porta_e;
  DDRH |= mask_porta_h;
  uint8_t intensity=0;
    
    ADC_init();
    while(1){
        while (! interrupt_occurred);
        interrupt_occurred=0;
        //ogni volta che occorre un'interruzione da parte del timer 5 cambiom il valore dell'output
        //compare register relativi ai pin campionati
        OCR1CL = intensity;
        OCR4BL = intensity;
        OCR3BL = intensity;
        intensity+=8;
        oscilloscope();
        //campiono per 60 secondi
        if(int_count >(60000/f)) return 0;
    }
}