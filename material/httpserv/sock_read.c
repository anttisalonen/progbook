#include <stdio.h>
#include <string.h>
#include <unistd.h>

void handle_client(int fd)
{
	for(;;) {
		char buf[1024];
		memset(buf, 0x00, sizeof(buf));
		int ret = read(fd, buf, 1023);
		if(ret == 0) {
			return;
		}
		if(ret == -1) {
			perror("read");
			return;
		}
		printf("%s\n", buf);
	}
}
