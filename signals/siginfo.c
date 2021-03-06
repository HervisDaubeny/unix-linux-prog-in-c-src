/*
 * POSIX.1b extension (aka POSIX.4) defines real time signals that address some
 * shortcomings of the original signals whose functionality is not changed by
 * the extension.  For those new signals, additional information is provided to
 * the signalled process.  Among that is a PID of the signalling process and its
 * UID. Example:
 *
 * $ ./a.out 		# and now kill 4114 with SIGINT ("kill -INT 4114")
 * PID 4114 started...
 * sig received: TERM
 * signalled by PID: 3764
 * and process was owned by: janp
 *
 * While the standard apparently does not define it, it seems to be possible to
 * provide additional information for "old" signals as well, if the new
 * mechanism is used.  That is what Solaris and Linux do and that is why it
 * works for SIGINT.  You can replace it with SIGRTMIN which is one of the
 * signals that the extension requires, for example, and see how it works.
 *
 * There are issues on macOS, see below.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#define	_XOPEN_SOURCE	700

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>
#include <pwd.h>

#ifndef _POSIX_REALTIME_SIGNALS
#error	"Sorry, POSIX.4 real time signals extensions missing."
#endif

/*
 * macOS does not define SIGRT* signals while it claims it supports
 * _POSIX_REALTIME_SIGNALS.  I'm not sure macOS is behaving properly here.
 * Also, after sending SIGINT via kill(1), sig_code is still 0.
 */
#if defined(__APPLE__)
#warning Only tested on Solaris and Linux.  Not fully functional on macOS.
#endif

/*
 * NOTE: this is not a safe signal handler since it is using unsafe functions
 *	 like printf/fflush.
 */
void
sig_handler(int sig, siginfo_t *info, void *ignored)
{
	/* Ctrl-C on Solaris makes info to be NULL pointer. */
	if (info == NULL) {
		printf("NULL signal info, we only know the signal number\n");
		printf("sig received: %s\n", strsignal(sig));
		return;
	}

	/*
	 * Was a signal generated by a user process? See a man page for
	 * siginfo.h(3HEAD) on Solaris for more information, or the spec.
	 *
	 * Note that SIGTERM should never be received other than from a user
	 * process as that is what the signal is for.  If curious, see what
	 * exactly si_code is for and what it can carry.  Also, you may want to
	 * read more about sigqueue(3C) with its 2nd parameter.
	 */
	if (info->si_code == SI_USER) {
		printf("sig received: %s\n", strsignal(info->si_signo));
		printf("signalled by PID: %d\n", info->si_pid);
		printf("and process was owned by: %s\n",
		    (getpwuid(info->si_uid))->pw_name);
	} else {
		printf("signal not generated from a user process.\n");
		printf("that means there is no PID nor UID.\n");
		printf("info->sig_code was %d.\n", info->si_code);
	}

	fflush(stdout);
}

int
main(void)
{
	struct sigaction act = { 0 };

	printf("PID %d started...\n", getpid());

	act.sa_sigaction = sig_handler;
	/*
	 * Must use this in order to use sa_sigaction field newly defined by the
	 * POSIX.4 extension.
	 */
	act.sa_flags = SA_SIGINFO;
	/*
	 * If you ^C it from the terminal, you will get no PID/UID.  That is
	 * because SIGINT is generated from the pseudo terminal driver (ie,
	 * kernel), not from a user process.
	 */
	(void) sigaction(SIGINT, &act, NULL);

	/* Wait for a signal, anything else than SIGINT will terminate us. */
	while (1) {
		pause();
	}

	return (0);
}
