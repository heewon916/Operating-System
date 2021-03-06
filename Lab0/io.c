// Figure 2.6: A program that does I/O (io.c)

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	int fd = open("test.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
	assert(fd > -1);
	int rc = write(fd, "hello world\n", 13);
	close(fd);
	return 0;
}
