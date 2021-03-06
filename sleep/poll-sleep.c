/*
 * More granular sleep using poll(). See also nanosleep.c.
 *
 * Note: this trick does not work on macOS (10.12.1)
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <poll.h>

#define	CYCLES	1000

int
main(void)
{
	int i;

	for (i = 0; i < CYCLES; ++i) {
		putc('.', stderr);

		/* Wait 50 miliseconds between the dots. */
		poll(NULL, 0, 50);

		/* Make it nice. */
		if ((i + 1) % 19 == 0)
			putc('\n', stderr);
	}

	return (0);
}
