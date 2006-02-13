# configure.ac - process with autoconf to create configure script

AC_INIT(cuetools,1.3,sorensen@users.berlios.de)
AM_INIT_AUTOMAKE($PACKAGE_NAME,$PACKAGE_VERSION)
# getting conflicting errors about config.h.in
#AM_CONFIG_HEADER(config.h)
#AC_CONFIG_HEADERS([config.h])

AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC

AC_CONFIG_FILES([Makefile doc/Makefile src/Makefile src/lib/Makefile src/tools/Makefile extras/Makefile])
AC_OUTPUT
