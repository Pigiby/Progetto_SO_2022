#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main() {
	int fd, len;
	char text[400];
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

	len=1;
    sleep(5);
	/* Read from serial port */
    int i;
    FILE * fptr;
    fptr = fopen("data.txt", "w"); // "w" defines "writing mode"
    L:
	memset(text, 0, 400);
	len = read(fd, text, 400);
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
    sleep(1);
    goto L;
}