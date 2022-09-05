#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>

int main(){
	int fd, len;
	char text[2048];
	struct termios options; /* Serial ports setting */

	fd = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
	if(tcgetattr(fd,&options)!=0){
        printf("Error %i from tcgetattr: %s\n",errno,strerror(errno));
        return 1;
    }

	/* Read current serial port settings */
	// tcgetattr(fd, &options);
	
	/* Set up serial port */
	options.c_cflag = B19200 | CS8 | CLOCAL|CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
   if(tcsetattr(fd,TCSANOW,&options)!=0){

        printf("Error %i from tcgetattr: %s\n",errno,strerror(errno));
        return 1;
    }
	/* Write to serial port */
    char msg[2048];
    float f=0.0;
    printf("ogni quanti millisecondi campionare (inserire un numero tra 100 e 4194, se il numero inserito è maggiore di 4194 o minore di 100 verrà scelto uno di default):");
    scanf("%f", &f );
	if( f < 100) f = 100;
	if(f > 4194) f = 4000;
    gcvt(f, 3, msg);
    strcat(msg,"\n");
	strcpy(text, msg);
	len = strlen(msg);
	len = write(fd, text, len);

	printf("Wrote %d bytes over UART\n", len);

	printf("You have 5s to send me some input data...\n");
	float sleeping = (f/1000);
	printf("%f",sleeping);
	sleep(5);
    int i;
    FILE * fptr;
    fptr = fopen("data.txt", "w"); // "w" defines "writing mode"
	len =  1;
    while(len != 0){
		memset(text, 0, 2048);
		len = read(fd, text, 2048);
		if(len==0 || len==-1){
			fclose(fptr);
			close(fd);
			return 0;
			}
				for (i = 0; i<strlen(text); i++) {
					fputc(text[i], fptr);
					if(text[i]==EOF) return 0;
				}
		printf("Received %d bytes\n", len);
		printf("Received string: %s\n", text);

		sleep(sleeping+1);
    }
}