# Progetto_SO_2022
## Oscilloscopio: definizione
L'oscilloscopio è uno strumento di misura elettronico che consente di visualizzare, su un grafico bidimensionale, l'andamento nel dominio del tempo dei segnali elettrici ed effettuare misure a lettura diretta di tensione (rappresentata sull'asse verticale) e periodo (con il tempo rappresentato sull'asse orizzontale).


Per fare ciò ho utilizzato la scheda ATMEGA 2560 e alcune sue funzionalità: AnalogDigitalConverter, Timer, PWM, UART che vedremo nel dettaglio. Tutto ciò è stato implementato in C.

### 1)ADC
Per prima cosa ci occupiamo della conversione. L'ADC converte un voltaggio analogico in input in un valore digitale a 10 bit attraverso successive approssimazioni. Il minimo valore è rappresentato dal ground mentre il valore massimo dipende dalla scheda utilizzata (solitamente 5V). I canali scelti (oppure solo uno) viene selezionato scrivendo i MUX bit negli indirizzi ADMUX e ADCSRB. Nel mio caso ho utilizzato 3 canali ADC1, ADC3 e ADC5, seguendo le indicazioni del datasheet ho settato opportunamente i registri a seconda di quale canale stessi convertendo:
ADMUX |= 1 << MUX0 |= 1 << REFS0 (channel 1)
ADMUX |= 1 << MUX0 |= 1 << MUX1 |= 1<< REFS0 (channel 3)
ADMUX |= 1 << MUX0 |= 1 << MUX2 |= 1<< REFS0 (channel 5)
Per la conversione analogica ho utilizzato il metodo del Polling: aspetto che finisca la conversione di un canale per poi passare al prossimo.
Contiamo il numero delle misure e facciamo una media.
Dobbiamo accedere al bit ADSC bit in ADCSRA, così quando inizia la conversione il bit ADSC viene letto a 1 e quando finisce viene settato a 0. Per fare questo usiamo l'operatore and. 
(ADCSRA & 1<<ADSC) fino a che questo è vero la conversione va avanti.
In pseudo-codice:
for(k in n)
 ADCSRA |= 1<<ADSC inizia la conversione
 while(ADCSRA & 1<<ADSC) //polling
  sum = sum + ADC -> adc letta n volte
  ADC[i] = (sum*ADCv)/n
La conversione è data dalla seguente formula: ADC=Vin*1024/Vref. Devo trovare Vin quindi: Vin = sum(ADC)/n * Vref/1024.
  
### 2)Timer
  





