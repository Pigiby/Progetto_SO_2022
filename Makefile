#ho preso spunto dai makefile del corso

# put here the file containing the main routine
# to be uploaded on the avr
# you can add multiple files, they will be all generated

BINS=oscilloscope.elf

# put here the additional .o files you want to generate
# one .c file for each .o should be present
OBJS= ./avr_common/uart.o

# put here the additional header files needed for compilation
HEADERS= ./avr_common/uart.h

# the file below contains the actual rules

include ./avr_common/avr.mk

# !!!!!!!!!!!!!!!! MODIFY HERE !!!!!!!!!!!!!!!! 


