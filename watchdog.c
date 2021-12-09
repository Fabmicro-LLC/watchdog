/*
 * Watchdog 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/watchdog.h>

int fd;

static void watchdog_keep_alive(void)
{
	int timeleft;

	ioctl(fd, WDIOC_KEEPALIVE, 0);
	ioctl(fd, WDIOC_GETTIMELEFT, &timeleft);
	printf("The timeout is in %d seconds\n", timeleft);
}

void watchdog_stop(int sig)
{
	int flags;

	flags = WDIOS_DISABLECARD;
	ioctl(fd, WDIOC_SETOPTIONS, &flags);
	close(fd);
	fprintf(stderr, "Watchdog disabled.\n");
	fflush(stderr);
	exit(0);
}


int main(int argc, char *argv[])
{
	int flags;

	signal(SIGINT, watchdog_stop);
	signal(SIGTERM, watchdog_stop);

	fd = open("/dev/watchdog", O_WRONLY);

	if (fd == -1) {
		fprintf(stderr, "Watchdog device not enabled.\n");
		fflush(stderr);
		exit(-1);
	}

	int timeout = 5;
	ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
	timeout = 0;
	ioctl(fd, WDIOC_GETTIMEOUT, &timeout);
	printf("Watchdog timeout was set to %d seconds\n", timeout);


	flags = WDIOS_ENABLECARD;
	ioctl(fd, WDIOC_SETOPTIONS, &flags);

	fprintf(stderr, "Watchdog enabled and Ticking Away!\n");
	fflush(stderr);

	while(1) {
		watchdog_keep_alive();
		sleep(1);
	}
}

