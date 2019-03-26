#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([sendat], [1.0], [developer@ciclope.info])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/sendat.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lmosquitto':
AC_CHECK_LIB([mosquitto], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
#AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])


AC_CONFIG_FILES([Makefile src/Makefile src/sendat.sh src/sendat.conf])
AC_OUTPUT
