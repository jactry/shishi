# getdate.m4 serial 5
dnl Copyright (C) 2002, 2003, 2004 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

AC_DEFUN([gl_GETDATE],
[
  dnl Prerequisites of lib/getdate.h.
  AC_REQUIRE([AM_STDBOOL_H])
  AC_REQUIRE([gl_TIMESPEC])

  dnl Prerequisites of lib/getdate.y.
  AC_REQUIRE([gl_BISON])
  AC_REQUIRE([AC_HEADER_STDC])
  AC_CHECK_FUNCS_ONCE(isascii)
  AC_STRUCT_TIMEZONE
  AC_REQUIRE([gl_CLOCK_TIME])
  AC_REQUIRE([gl_TM_GMTOFF])
])
