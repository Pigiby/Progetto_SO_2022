#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

int main() {
	int fd, len;
	char text[300];
	struct termios options; /* Serial ports setting */

	fd = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
	if (fd < 0) {
		perror("Error opening serial port");
		return -1;
	}

	/* Read current serial port settings */
	// tcgetattr(fd, &options);
	
	/* Set up serial port */
	options.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	/* Apply the settings */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);

	/* Write to serial port */
    char msg[300];
    float f=0.0;
    printf("inserisci una frequenza compresa tra 1 e 32766 :");
    scanf("%f", &f );
    gcvt(f, 3, msg);
    strcat(msg,"\n");
	strcpy(text, msg);
	len = strlen(text);
	len = write(fd, text, len);

	printf("Wrote %d bytes over UART\n", len);

	printf("You have 5s to send me some input data...\n");
	sleep(5);
    int i;
    FILE * fptr;
    fptr = fopen("data.txt", "w"); // "w" defines "writing mode"
    L:
	memset(text, 0, 300);
	len = read(fd, text, 300);
    if(len==0){
        fclose(fptr);
        close(fd);
	    return 0;
        }
            for (i = 0; i<strlen(text); i++) {
                fputc(text[i], fptr);
                if(text[i]==EOF) goto L;
            }
	printf("Received %d bytes\n", len);
	printf("Received string: %s\n", text);
    sleep(f/100+1);
    goto L;
	/* Read from serial port */
}