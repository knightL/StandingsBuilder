AC_PREREQ([2.69])
AC_INIT([standingsbuilder], [1.1], [knightLA@yandex.ru])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES(standingsbuilder, libcurl libxml-2.0)
AC_SUBST(standingsbuilder_CFLAGS)
AC_SUBST(standingsbuilder_LIBS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
