AC_PREREQ(2.61)
AC_INIT(telephony_simulator, 1.0)

AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_CXX

AC_CHECK_PROGS(PKG_CONFIG, pkg-config, false)
AC_CHECK_PROGS(AR, ar, false)
#AC_PREFIX_DEFAULT(/opt/samsung_sdk)

if test "$PKG_CONFIG" = "false"
then
	AC_MSG_ERROR([pkg-config not present, please install])
fi

CFLAGS="${CFLAGS} -Wall"

AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)
GLIB_CFLAGS=`$PKG_CONFIG --cflags glib-2.0`
GLIB_LIBS=`$PKG_CONFIG --libs glib-2.0`

AC_SUBST(XML_CFLAGS)
AC_SUBST(XML_LIBS)
XML_CFLAGS=`$PKG_CONFIG --cflags libxml-2.0`
XML_LIBS=`$PKG_CONFIG --libs libxml-2.0`

AC_SUBST(GTHREAD_CFLAGS)
AC_SUBST(GTHREAD_LIBS)
GTHREAD_CFLAGS=`$PKG_CONFIG --cflags gthread-2.0`
GTHREAD_LIBS=`$PKG_CONFIG --libs gthread-2.0`

AC_CONFIG_FILES([
	Makefile
	lib/libcommon/Makefile
	lib/libsms/Makefile
	libvmodem/Makefile
	vmodem/Makefile])

AC_OUTPUT
