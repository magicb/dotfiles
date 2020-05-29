/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>
#include <locale.h>

#include "arg.h"
#include "slstatus.h"
#include "util.h"

struct arg {
	const char *(*func)();
	const char *fmt;
	const char *fmt_crit;
	const char *args;
	const int interval;
	const int critical;
};

struct argstat {
	int curint;
	char res[128];
};

char buf[1024];
static int done;
static Display *dpy;

#include "config.h"

static void
terminate(const int signo)
{
	(void)signo;

	done = 1;
}

static void
difftimespec(struct timespec *res, struct timespec *a, struct timespec *b)
{
	res->tv_sec = a->tv_sec - b->tv_sec - (a->tv_nsec < b->tv_nsec);
	res->tv_nsec = a->tv_nsec - b->tv_nsec +
	               (a->tv_nsec < b->tv_nsec) * 1E9;
}

static void
usage(void)
{
	die("usage: %s [-s]", argv0);
}

int toint(char const *s)
{
     if ( s == NULL || *s == '\0' )
		 return 0;

     while(*s)
     {
          if ( (*s >= '0' && *s <= '9') || *s == '+' || *s == '-' )
			  break;
          ++s;
     }

     int negate = (s[0] == '-');
     if ( *s == '+' || *s == '-' ) 
         ++s;

     if ( *s == '\0')
		 return 0;

     int result = 0;
     while(*s)
     {
          if ( *s >= '0' && *s <= '9' )
              result = result * 10  - (*s - '0');  //assume negative number
		  else
			  break;
          ++s;
     }
     return negate ? result : -result; //-result is positive!
} 

int
main(int argc, char *argv[])
{
	setlocale(LC_ALL,"ru_RU.UTF-8");
	struct sigaction act;
	struct timespec start, current, diff, intspec, wait;
	struct argstat argsstat[LEN(args)];
	size_t i, len;
	int sflag, ret;
	char status[MAXLEN];
	const char *res;
	const char *fmt;
	int val;

	sflag = 0;
	ARGBEGIN {
		case 's':
			sflag = 1;
			break;
		default:
			usage();
	} ARGEND

	if (argc) {
		usage();
	}

	memset(&act, 0, sizeof(act));
	act.sa_handler = terminate;
	sigaction(SIGINT,  &act, NULL);
	sigaction(SIGTERM, &act, NULL);

	if (!sflag && !(dpy = XOpenDisplay(NULL))) {
		die("XOpenDisplay: Failed to open display");
	}
	printf("{\"version\":1}[\n[]\n");

	for (i = 0; i < LEN(args); i++) {
		/* printf("%s %s %s %d %d\r\n", args[i].fmt, args[i].fmt_crit, args[i].args, args[i].interval, args[i].critical); */
		argsstat[i].curint = args[i].interval;
		if (!(res = args[i].func(args[i].args)))
			res = unknown_str;
		/* printf("%s\r\n", res); */
		memcpy(argsstat[i].res, res, strlen(res)+1);
	}

	while (!done) {
		if (clock_gettime(CLOCK_MONOTONIC, &start) < 0) {
			die("clock_gettime:");
		}

		status[0] = '\0';
		for (i = len = 0; i < LEN(args); i++) {
			//res = argsstat[i].res;
			val = 0;
			if (!(argsstat[i].curint == -1 || --(argsstat[i].curint) > 0)) {
				if (args[i].interval > 0)
					argsstat[i].curint = args[i].interval;
				if (!(res = args[i].func(args[i].args))) {
					res = unknown_str;
				}
				memcpy(argsstat[i].res, res, strlen(res)+1);
			}
			//printf("%d %s %s %p %p %d\n", argsstat[i].curint, res, argsstat[i].res, argsstat[i].res, res, strlen(res));
			val = toint(res);
			// printf("\t\t\t\t%d\r\n", val);
			if (args[i].fmt_crit != NULL && val != 0 && ( (args[i].critical > 0 && val > args[i].critical) || (args[i].critical < 0 && val < args[i].critical*-1)))
				fmt = args[i].fmt_crit;
			else
				fmt = args[i].fmt;

			if ((ret = esnprintf(status + len, sizeof(status) - len, fmt, argsstat[i].res)) < 0) {
				break;
			}
			len += ret;
		}

		if (sflag) {
			puts(status);
			fflush(stdout);
			if (ferror(stdout))
				die("puts:");
		} else {
			if (XStoreName(dpy, DefaultRootWindow(dpy), status)
                            < 0) {
				die("XStoreName: Allocation failed");
			}
			XFlush(dpy);
		}

		if (!done) {
			if (clock_gettime(CLOCK_MONOTONIC, &current) < 0) {
				die("clock_gettime:");
			}
			difftimespec(&diff, &current, &start);

			intspec.tv_sec = interval / 1000;
			intspec.tv_nsec = (interval % 1000) * 1E6;
			difftimespec(&wait, &intspec, &diff);

			if (wait.tv_sec >= 0) {
				if (nanosleep(&wait, NULL) < 0 &&
				    errno != EINTR) {
					die("nanosleep:");
				}
			}
		}
	}

	if (!sflag) {
		XStoreName(dpy, DefaultRootWindow(dpy), NULL);
		if (XCloseDisplay(dpy) < 0) {
			die("XCloseDisplay: Failed to close display");
		}
	}

	return 0;
}
