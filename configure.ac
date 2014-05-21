#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(zuc, 0.0.1, liyang.jasper@gmail.com)
AM_INIT_AUTOMAKE([1.10 -Wall])
AC_OUTPUT(Makefile src/Makefile doc/Makefile examples/Makefile)

AM_PROG_AR
AC_PROG_CC
AC_PROG_CXX
LT_INIT()

# Checks for programs.

AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h inttypes.h limits.h netinet/in.h stddef.h stdint.h stdlib.h string.h strings.h sys/file.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit floor ftruncate gettimeofday memset mkdir pow select setenv strchr strerror])



AC_OUTPUT
