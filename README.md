# Progetto_SO_2022

PARTI IMPORTANTI DOCUMENTAZIONE ATMEL

The analog input channel is selected by writing to the MUX bits in ADMUX and ADCSR.

If differential channels are selected, the voltage difference between the selected input channel pair then becomes
the analog input to the ADC. If single ended channels are used, the amplifier is bypassed altogether.

The ADC is enabled by setting the ADC Enable bit, ADEN in ADCSRA. Voltage reference and input channel selections will not go into effect until ADEN is set.

The ADC generates a 10-bit result which is presented in the ADC Data Registers, ADCH and ADCL. By default,
the result is presented right adjusted, but can optionally be presented left adjusted by setting the ADLAR bit in
ADMUX.

If the result is left adjusted and no more than 8-bit precision is required, it is sufficient to read ADCH. Otherwise,
ADCL must be read first, then ADCH, to ensure that the content of the Data Registers belongs to the same conver-
sion.


