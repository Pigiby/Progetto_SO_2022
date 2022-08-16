#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "./avr_common/uart.h"

/* per iniziare una conversione bisogna scrivere un uno logico sull'ADC start conversion bit, ADSC