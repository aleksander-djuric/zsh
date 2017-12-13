AC_PREREQ([2.69])
AC_INIT([zsh], [1.0])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([zsh.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lreadline':
AC_CHECK_LIB([readline], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([dup2 getcwd gethostname setenv strchr strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
