#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>

#define DEV_NAME "/dev/mydev"

struct ioctl_cmd {
	unsigned int reg;
	unsigned int offset;
	unsigned int val;
};

#define IOC_MAGIC 'd'

#define IOCTL_VALGET _IOR(IOC_MAGIC, 2, struct ioctl_cmd)
#define IOCTL_VALSET _IOW(IOC_MAGIC, 1, struct ioctl_cmd)

int main(int argc, char *argv[])
{
	int fd, val = 0;
	struct ioctl_cmd data;

	printf("*+*+*+*+*+ main() START argc = %d *+*+*+*+*+\n", argc);

	if ((fd = open(DEV_NAME, O_RDWR)) < 0) {
		printf("Failed to open : %s\n", strerror(errno));
		return -1;
	}
	usleep(5000000);
	if (argc < 2) {
		printf("Get command\n");
		if (ioctl(fd, IOCTL_VALGET, &data) < 0) {
			printf("Failed to ioctl : %s\n", strerror(errno));
			close(fd);
			return -1;
		}
		printf("val = %d\n", data.val);
	} else if (argc == 2) {
		printf("Set command\n");
		data.val = atoi(argv[1]);
		if (ioctl(fd, IOCTL_VALSET, &data) < 0) {
			printf("Failed to ioctl : %s\n", strerror(errno));
			close(fd);
			return -1;
		}
		usleep(5000000);
		data.val = 0;
		if (ioctl(fd, IOCTL_VALSET, &data) < 0) {
			printf("Failed to ioctl : %s\n", strerror(errno));
			close(fd);
			return -1;
		}
	}
	close(fd);
	return 0;
}
