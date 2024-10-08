#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(FreeXL, 2.1.0-devel, a.furieri@lqt.it)
AC_LANG(C)
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_CONFIG_HEADERS(config.h)

# config depending options
AH_TEMPLATE([OMIT_XMLDOC],
            [Should be defined in order to disable XML documents support.])


# Checks for header files.
AC_CHECK_HEADERS(stdlib.h,, [AC_MSG_ERROR([cannot find stdlib.h, bailing out])])
AC_CHECK_HEADERS(stdio.h,, [AC_MSG_ERROR([cannot find stdio.h, bailing out])])
AC_CHECK_HEADERS(string.h,, [AC_MSG_ERROR([cannot find string.h, bailing out])])
AC_CHECK_HEADERS(math.h,, [AC_MSG_ERROR([cannot find math.h, bailing out])])


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT(win32-dll)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MEMCMP
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([sqrt strcasecmp strerror strncasecmp strstr strerror])

# gcov support
AC_ARG_ENABLE(gcov, AS_HELP_STRING([--enable-gcov],[turn on code coverage analysis tools]))
if test "x$enable_gcov" = "xyes"; then
    dnl Check for -fprofile-arcs and -ftest-coverage option
    CFLAGS=$CFLAGS" -fprofile-arcs -ftest-coverage -g"
    GCOV_FLAGS="-lgcov"
fi
 
AC_CONFIG_FILES([Makefile \
		headers/Makefile \
		src/Makefile \
		tests/Makefile \
		examples/Makefile \
		Doxyfile \
		freexl.pc])

AC_CHECK_HEADERS(iconv.h,, [AC_MSG_ERROR([cannot find iconv.h, bailing out])])
# on some systems "iconv()" lives in libc. On others it lives in libiconv
#  on older systems "libiconv()" lives in libiconv
AC_SEARCH_LIBS(iconv,iconv,,
	AC_SEARCH_LIBS(libiconv,iconv,,AC_MSG_ERROR(['libiconv' is required but it doesn't seem to be installed on this system.]),))
AC_SEARCH_LIBS(locale_charset,charset,,
	AC_SEARCH_LIBS(nl_langinfo,c,,AC_MSG_ERROR(['libcharset' is required but it doesn't seem to be installed on this system.]),))

#-----------------------------------------------------------------------
#   --enable-xmldocs
#
AC_ARG_ENABLE(xmldocs, [AS_HELP_STRING(
  [--enable-xmldocs], [enables XML documents support [default=yes]])],
  [], [enable_xmldocs=yes])
if test x"$enable_xmldocs" != "xyes"; then
  AC_DEFINE(OMIT_XMLDOC)
else
AC_CHECK_HEADERS(minizip/unzip.h,, [AC_MSG_ERROR([cannot find minizip/unzip.h, bailing out])])
AC_SEARCH_LIBS(unzLocateFile, minizip,,AC_MSG_ERROR(['libminizip' is required but it doesn't seem to be installed on this system.]))

AC_CHECK_HEADERS(expat.h,, [AC_MSG_ERROR([cannot find expat.h, bailing out])])
AC_CHECK_LIB(expat,XML_ParserCreate,,AC_MSG_ERROR(['expat' is required but it doesn't seem to be installed on this system.]))
fi
#-----------------------------------------------------------------------

AC_OUTPUT
