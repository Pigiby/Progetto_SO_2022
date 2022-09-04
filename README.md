# Progetto_SO_2022
## Oscilloscopio: definizione
L'oscilloscopio è uno strumento di misura elettronico che consente di visualizzare, su un grafico bidimensionale, l'andamento nel dominio del tempo dei segnali elettrici ed effettuare misure a lettura diretta di tensione (rappresentata sull'asse verticale) e periodo (con il tempo rappresentato sull'asse orizzontale).


Per fare ciò ho utilizzato la scheda ATMEGA 2560 e alcune sue funzionalità: AnalogDigitalConverter, Timer, PWM, UART. Tutto ciò è stato implementato in C.

### 1)ADC
Per prima cosa ci occupiamo della conversione. L'ADC converte un voltaggio analogico in input in un valore digitale a 10 bit attraverso successive approssimazioni. Il minimo valore è rappresentato dal ground mentre il valore massimo dipende dalla scheda utilizzata (solitamente 5V). I canali scelti (oppure solo uno) viene selezionato scrivendo i MUX bit negli indirizzi ADMUX e ADCSRB. Nel mio caso ho utilizzato 3 canali ADC1, ADC3 e ADC5, seguendo le indicazioni del datasheet ho settato opportunamente i registri a seconda di quale canale stessi convertendo:
```C
ADMUX |= 1 << MUX0 |= 1 << REFS0 (channel 1)

ADMUX |= 1 << MUX0 |= 1 << MUX1 |= 1<< REFS0 (channel 3)

ADMUX |= 1 << MUX0 |= 1 << MUX2 |= 1<< REFS0 (channel 5)
```


Per la conversione analogica ho utilizzato il metodo del Polling: aspetto che finisca la conversione di un canale per poi passare al prossimo.
Contiamo il numero delle misure e facciamo una media.
Dobbiamo accedere al bit ADSC bit in ADCSRA, così quando inizia la conversione il bit ADSC viene letto a 1 e quando finisce viene settato a 0. Per fare questo usiamo l'operatore and. 
(ADCSRA & 1<<ADSC) fino a che questo è vero la conversione va avanti.
In pseudo-codice:
```C
for(k in n)
 ADCSRA |= 1<<ADSC inizia la conversione
   while(ADCSRA & 1<<ADSC) //polling
    sum = sum + ADC -> adc letta n volte
    ADC[i] = (sum*ADCv)/n //canale convertito
```


***
 La conversione è data dalla seguente formula: ADC=Vin*1024/Vref. Devo trovare Vin quindi: Vin = sum(ADC)/n * Vref/1024.
  
### 2)Timer e PWM
Per questa parte del progetto ho usato i pdf usati a lezione (possiamo trovarli all'interno della cartella 'risorse' di questa repo) e il codice relativo ai timer sempre visto durante le lezioni. In particolare ho utilizzato quattro timer, I in cui i timer 1,3,4 sono stati utilizzati in PWM. Con questi 3 timer ho generato 3 forme d'onde diverse, la differenza è data dal prescaler.
```C
#define TCCRA_1_MASK (1<<WGM10)|(1<<COM1C0)|(1<<COM1C1) 
#define TCCRB_1_MASK ((1<<WGM12)|(1<<CS10)) 
#define TCCRA_3_MASK (1<<WGM30)|(1<<COM3B0)|(1<<COM3B1)
#define TCCRB_3_MASK ((1<<WGM32)|(1<<CS30)|(1<<CS31))
#define TCCRA_4_MASK (1<<WGM40)|(1<<COM4B0)|(1<<COM4B1)
#define TCCRB_4_MASK ((1<<WGM42)|(1<<CS42))
```
I COMnA1:0, COMnB1:0, e COMnC1:0 controllano gli outcompare pins (OCnA, OCnB, e OCnC rispettivamente
WGMn1:0 combinato con WGMn3:2 del registro TTCRnB controllano la sequenza del contatore e dicono quale tipo di forma d'onda viene è stata generata.

Dal datasheet
"CSn2:0 The three clock select bits select the clock source to be used by the Timer/Counter".

Utilizzo gli 8 bit inferiori degli output compare registers e quindi setto a 0 tutti quelli superiori.
```C
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
    ```
Il timer 5 è stato utilizzato per campionare i 3 canali, ogni volta che questo timer raggiunge il valore dell'OCR viene inviata un interrupt che viene gestita dall'interrupt service routine che provvederà a incrementare alcune variabili.

### 3) UART



