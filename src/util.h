
#include <sys/types.h>		/* some systems still require this */
#include <sys/stat.h>
#include <sys/termios.h>	/* for winsize */
#if defined(MACOS) || !defined(TIOCGWINSZ)
#include <sys/ioctl.h>
#endif

#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */


#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>		/* ISO C variable aruments */
#include <sys/time.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


#define	MAXLINE	4096			/* max line length */

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)


void	 clr_fl(int, int);
void	 set_fl(int, int);					/* {Prog setfl} */

ssize_t	 writen(int, const void *, size_t);	/* {Prog readn_writen} */
ssize_t  readn(int, void *ptr, size_t n);
ssize_t  readline(int fd, void *buffer, size_t n);

void	err_msg(const char *, ...);			/* {App misc_source} */
void	err_sys(const char *, ...) __attribute__((noreturn));

void	 sleep_us(unsigned int);			/* {Ex sleepus} */
void     sleep_ms(unsigned int);


int Daemonize(char* daemonname);

