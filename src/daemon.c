#include <sys/stat.h>
#include <fcntl.h>
#include "util.h"
#include <stdio.h>
#include <strings.h>
#include <signal.h>

const char* getdwd(char* suffix);

// argv[0] as parameter
void setdaemonname(char* app)
{
    char* appname = NULL;
    appname = rindex(app,'/');
    if(!appname)
        appname = app;
    else
        appname += 1;
    setenv("DAEMONNAME", appname, 1); // don't change it if it is already set
}

#define BUF_SIZE 64
#define PATH_SIZE 128
#define PRIVILIGED_PATH_PREFIX "/run/"
#define NORMAL_PATH_PRIFIX "/tmp/"
#define PIDFILE "PID"
static int pidfd = -1;

// daemon working directory
const char* getdwd(char* suffix)
{
    const char* daemon_name = getenv("DAEMONNAME");
    static char workingpath[PATH_SIZE];
    const char* pathprefix = NULL;

    if(!daemon_name)
    {
        fprintf(stderr, "can't get env DAEMONNAME\n");
        return NULL;
    }
    if(!suffix)
        suffix="";
    if(geteuid() != 0)
    {
        pathprefix = NORMAL_PATH_PRIFIX;
    }
    else //root
    {
        pathprefix = PRIVILIGED_PATH_PREFIX;
    }

    snprintf(workingpath, PATH_SIZE - 1, "%s%s/%s", pathprefix, daemon_name, suffix);
    // workingpath[PATH_SIZE-1] = 0;

    return workingpath;
}

// 0 on success, -1 failed
int ensuredir(const char* dirpath)
{
    if ((!dirpath) || (*dirpath == 0))
    {
        /* code */
        fprintf(stderr,"bad dir path\n");
    }
    //ensure dirpath exist 
    int result = mkdir(dirpath, DIR_MODE);
    if(result == 0) // create success
    {
        return result;
    }
    else if(errno == EEXIST)
    {
        struct stat s;
        result = stat(dirpath, &s);
        if(-1 == result) {
            err_msg("cant' stat %s\n", dirpath);
        } else {
            if(!S_ISDIR(s.st_mode)) {
                /* exists but is no dir */
                fprintf(stderr,"%s exists but not a dir\n", dirpath);
                result = -1;
            }
            else
            {
                result = 0; // dirpath already exit
            }
        }

    }
    return result;
}


void
lockpidfile()
{
    int fd;
    char buf[BUF_SIZE];

    fd = open(PIDFILE, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Daemon already running ");
        _exit(-1);
    }

    // if (ftruncate(fd, 0) == -1)
    //     perror("Could not truncate PID file" );

    snprintf(buf, BUF_SIZE, "%ld\n", (long) getpid());
    if (write(fd, buf, strlen(buf)) != strlen(buf))
        perror("Writing to PID file failed" );
    // close(fd);
    pidfd = fd;
    return;
}

void finish()
{
    // release lock
    // delete PID file
    if(pidfd != -1)
    {
        close(pidfd);
    }
    unlink(PIDFILE);
}

void sig_handler(int signum)  
{  
    printf("terminated by signal %d\n", signum);
    finish();
    _exit(-1);
}  

int                                     /* Returns 0 on success, -1 on error */
Daemonize(char* daemonname)
{
    int maxfd, fd, pid;
    setdaemonname(daemonname);

    pid = fork();
    if(pid == -1)
    {
        err_msg("fork error\n");
        return -1;
    }
    else if (pid > 0)  // parent
    {
        _exit(EXIT_SUCCESS);  // exit successfully
    }
    
    if (setsid() == -1)                  //Become leader of new session 
        return -1;

    // again
    pid = fork();
    if(pid == -1)
    {
        err_msg("fork error\n");
        return -1;
    }
    else if (pid > 0)  // parent
    {
        _exit(EXIT_SUCCESS);  // exit successfully
    }
    
    umask(0);                       /* Clear file mode creation mask */
    ensuredir(getdwd(NULL));
    chdir(getdwd(NULL));                     /* Change to working directory */

    /* Close all open files */
    maxfd = sysconf(_SC_OPEN_MAX);
    if (maxfd == -1)                /* Limit is indeterminate... */
        maxfd = 8192;       /* so take a guess */

    for (fd = 3; fd < maxfd; fd++)
        close(fd);

    lockpidfile();
    

    close(STDIN_FILENO);            /* Reopen standard fd's to /dev/null */

    fd = open("/dev/null", O_RDWR);

    if (fd != STDIN_FILENO)         /* 'fd' should be 0 */
    {
        perror("redirect stdin to /dev/null failed");
        return -1;
    }

    // stdout
    close(STDOUT_FILENO);
    fd = open(getdwd("stdout") , O_RDWR | O_CREAT | O_TRUNC, FILE_MODE);
    if (fd != STDOUT_FILENO)         /* 'fd' should be 1 */
    {
        perror("redirect stdout failed");
        return -1;
    }
    setlinebuf(stdout);
    // if (ftruncate(fd, 0) == -1)
    // {
    //     perror("truncate stdout error");
    //     return -1;
    // }

    // stderr
    close(STDERR_FILENO);
    fd = open(getdwd("stderr") , O_RDWR | O_CREAT | O_TRUNC, FILE_MODE);

    if (fd != STDERR_FILENO)         /* 'fd' should be 2 */
    {
        perror("redirect stderr failed\n");
        return -1;
    }  

    // if (ftruncate(fd, 0) == -1)
    // {
    //     perror("truncate stderr failed");
    //     return -1;
    // }
    signal(SIGTERM, sig_handler);
    atexit(finish);
    return 0;
}


#if _SELFTESTING
int
main(int argc, char *argv[])
{
    Daemonize(argv[0]);
    printf("%d\n", geteuid());
    printf("%d\n", getpid());
    printf("%d\n", geteuid());
    printf("%d\n", getpid());
    // fflush(NULL);
    sleep(300);
    return 0;
}
#endif

// create working dir according to user in
// create pid, stdout, stderr, lock, etc
// fork and redirect