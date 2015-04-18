AC_PREREQ([2.69])
AC_INIT([standingsbuilder], [1.3], [knightLA@yandex.ru])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([src/config.h])

AC_ARG_WITH([libcurl],
    AS_HELP_STRING([--without-libcurl], [do not use libcurl]))

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AS_IF([test "x$with_libcurl" != "xno"],[
	PKG_CHECK_MODULES(libcurl, libcurl)
	AC_SUBST(libcurl_CFLAGS)
	AC_SUBST(libcurl_LIBS)
	AC_DEFINE([HAVE_LIBCURL], [1],
            [Define to 1 if you have libcurl installed])
])

PKG_CHECK_MODULES(libxml, libxml-2.0)
AC_SUBST(libxml_CFLAGS)
AC_SUBST(libxml_LIBS)
PKG_CHECK_MODULES(lua5_2, lua5.2)
AC_SUBST(lua5_2_CFLAGS)
AC_SUBST(lua5_2_LIBS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
