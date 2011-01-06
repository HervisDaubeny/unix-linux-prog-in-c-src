/*
 * Implement this command:
 *
 *	cat <in >out 
 *
 * Which means, that after running "./a.out" the program will copy the existing
 * file "in" to a newly created (or an existing truncated) file "out".
 */
#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

int
main(void)
{
	int fd;

	if ((fd = open("in", O_RDONLY)) == -1)
		err(1, "open");

	/* Do the "<in" part. */
	close(0);
	dup(fd);

	/* Do the ">out" part. */
	close(1);
	if (creat("out", 0666) == -1)
		err(1, "creat");

	execl("/bin/cat", "cat", NULL);
	err(1, "execl");

	/* Not reached. */
	return (1);
}