#include <stdio.h>
int main() {
        int i;
        FILE * fptr;
        char fn[50];
        char str[] = "example for oscilloscope\n";
        for(int j=0;j<3;j++){
            if(j==0)
                fptr = fopen("data_channel_one", "w"); // "w" defines "writing mode"
            else if(j==1)
                fptr = fopen("data_channel_two", "w"); // "w" defines "writing mode"
            else
                fptr = fopen("data_channel_three", "w"); // "w" defines "writing mode"
            /* write to file using fputc() function */
            for (i = 0; str[i] != '\n'; i++) {
                fputc(str[i], fptr);
        }
        fclose(fptr);
        }
        return 0;
    }