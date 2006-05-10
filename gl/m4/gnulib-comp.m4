# Copyright (C) 2004-2006 Free Software Foundation, Inc.
#
# This file is free software, distributed under the terms of the GNU
# General Public License.  As a special exception to the GNU General
# Public License, this file may be distributed as part of a program
# that contains a configuration script generated by Autoconf, under
# the same distribution terms as the rest of that program.
#
# Generated by gnulib-tool.
#
# This file represents the compiled summary of the specification in
# gnulib-cache.m4. It lists the computed macro invocations that need
# to be invoked from configure.ac.
# In projects using CVS, this file can be treated like other built files.


# This macro should be invoked from ./configure.ac, in the section
# "Checks for programs", right after AC_PROG_CC, and certainly before
# any checks for libraries, header files, types and library functions.
AC_DEFUN([gl_EARLY],
[
  AC_REQUIRE([AC_PROG_RANLIB])
  AC_REQUIRE([AC_GNU_SOURCE])
  AC_REQUIRE([gl_USE_SYSTEM_EXTENSIONS])
])

# This macro should be invoked from ./configure.ac, in the section
# "Check for header files, types and library functions".
AC_DEFUN([gl_INIT],
[
  AM_CONDITIONAL([GL_COND_LIBTOOL], [true])
  gl_FUNC_ALLOCA
  gl_ALLOCSA
  gl_FUNC_BASE64
  gl_CRC
  gl_ERROR
  dnl gl_USE_SYSTEM_EXTENSIONS must be added quite early to configure.ac.
  gl_GC
  gl_GC_ARCFOUR
  gl_GC_DES
  gl_GC_HMAC_MD5
  gl_GC_HMAC_SHA1
  gl_GC_MD4
  gl_GC_MD5
  gl_GC_PBKDF2_SHA1
  gl_GC_RANDOM
  gl_GETADDRINFO
  gl_GETDATE
  gl_FUNC_GETDELIM
  gl_FUNC_GETDOMAINNAME
  gl_FUNC_GETHOSTNAME
  gl_FUNC_GETLINE
  gl_GETOPT
  gl_FUNC_GETPASS
  gl_FUNC_GETSUBOPT
  gl_GETTIME
  AC_FUNC_GETTIMEOFDAY_CLOBBER
  gl_MBCHAR
  gl_MBITER
  gl_FUNC_MEMCHR
  gl_FUNC_MKTIME
  gl_FUNC_READLINK
  AC_FUNC_REALLOC
  gl_C_RESTRICT
  gt_FUNC_SETENV
  gl_SIZE_MAX
  gl_TYPE_SOCKLEN_T
  gt_TYPE_SSIZE_T
  gl_STDARG_H
  AM_STDBOOL_H
  gl_STDINT_H
  gl_STRCASE
  gl_FUNC_STRCHRNUL
  gl_FUNC_STRDUP
  gl_FUNC_STRNDUP
  gl_FUNC_STRNLEN
  gl_FUNC_STRTOK_R
  gl_FUNC_STRVERSCMP
  gl_HEADER_SYS_SOCKET
  gl_TIME_R
  gl_FUNC_TIMEGM
  gl_TIMESPEC
  gl_HEADER_UNISTD
  gl_FUNC_VASNPRINTF
  gl_FUNC_VASPRINTF
  gl_XALLOC
  gl_XREADLINK
  gl_XSIZE
  gl_XSTRNDUP
  gl_XVASPRINTF
])

# This macro records the list of files which have been installed by
# gnulib-tool and may be removed by future gnulib-tool invocations.
AC_DEFUN([gl_FILE_LIST], [
  build-aux/GNUmakefile
  build-aux/config.rpath
  build-aux/maint.mk
  doc/getdate.texi
  lib/alloca_.h
  lib/allocsa.c
  lib/allocsa.h
  lib/allocsa.valgrind
  lib/arcfour.c
  lib/arcfour.h
  lib/asnprintf.c
  lib/asprintf.c
  lib/base64.c
  lib/base64.h
  lib/crc.c
  lib/crc.h
  lib/des.c
  lib/des.h
  lib/error.c
  lib/error.h
  lib/gai_strerror.c
  lib/gc-gnulib.c
  lib/gc-libgcrypt.c
  lib/gc-pbkdf2-sha1.c
  lib/gc.h
  lib/getaddrinfo.c
  lib/getaddrinfo.h
  lib/getdate.h
  lib/getdate.y
  lib/getdelim.c
  lib/getdelim.h
  lib/getdomainname.c
  lib/getdomainname.h
  lib/gethostname.c
  lib/getline.c
  lib/getline.h
  lib/getopt.c
  lib/getopt1.c
  lib/getopt_.h
  lib/getopt_int.h
  lib/getpass.c
  lib/getpass.h
  lib/getsubopt.c
  lib/getsubopt.h
  lib/gettext.h
  lib/gettime.c
  lib/gettimeofday.c
  lib/hmac-md5.c
  lib/hmac-sha1.c
  lib/hmac.h
  lib/mbchar.c
  lib/mbchar.h
  lib/mbuiter.h
  lib/md4.c
  lib/md4.h
  lib/md5.c
  lib/md5.h
  lib/memchr.c
  lib/memxor.c
  lib/memxor.h
  lib/mktime.c
  lib/printf-args.c
  lib/printf-args.h
  lib/printf-parse.c
  lib/printf-parse.h
  lib/progname.c
  lib/progname.h
  lib/readlink.c
  lib/realloc.c
  lib/setenv.c
  lib/setenv.h
  lib/sha1.c
  lib/sha1.h
  lib/size_max.h
  lib/socket_.h
  lib/stdbool_.h
  lib/stdint_.h
  lib/strcase.h
  lib/strcasecmp.c
  lib/strchrnul.c
  lib/strchrnul.h
  lib/strdup.c
  lib/strdup.h
  lib/strncasecmp.c
  lib/strndup.c
  lib/strndup.h
  lib/strnlen.c
  lib/strnlen.h
  lib/strnlen1.c
  lib/strnlen1.h
  lib/strtok_r.c
  lib/strtok_r.h
  lib/strverscmp.c
  lib/strverscmp.h
  lib/time_r.c
  lib/time_r.h
  lib/timegm.c
  lib/timegm.h
  lib/timespec.h
  lib/unsetenv.c
  lib/vasnprintf.c
  lib/vasnprintf.h
  lib/vasprintf.c
  lib/vasprintf.h
  lib/xalloc.h
  lib/xasprintf.c
  lib/xgetdomainname.c
  lib/xgetdomainname.h
  lib/xgethostname.c
  lib/xgethostname.h
  lib/xmalloc.c
  lib/xreadlink.c
  lib/xreadlink.h
  lib/xsize.h
  lib/xstrndup.c
  lib/xstrndup.h
  lib/xvasprintf.c
  lib/xvasprintf.h
  m4/alloca.m4
  m4/allocsa.m4
  m4/arcfour.m4
  m4/base64.m4
  m4/bison.m4
  m4/clock_time.m4
  m4/crc.m4
  m4/des.m4
  m4/eealloc.m4
  m4/eoverflow.m4
  m4/error.m4
  m4/extensions.m4
  m4/gc-arcfour.m4
  m4/gc-des.m4
  m4/gc-hmac-md5.m4
  m4/gc-hmac-sha1.m4
  m4/gc-md4.m4
  m4/gc-md5.m4
  m4/gc-pbkdf2-sha1.m4
  m4/gc-random.m4
  m4/gc.m4
  m4/getaddrinfo.m4
  m4/getdate.m4
  m4/getdelim.m4
  m4/getdomainname.m4
  m4/gethostname.m4
  m4/getline.m4
  m4/getopt.m4
  m4/getpass.m4
  m4/getsubopt.m4
  m4/gettime.m4
  m4/gettimeofday.m4
  m4/hmac-md5.m4
  m4/hmac-sha1.m4
  m4/intmax_t.m4
  m4/inttypes.m4
  m4/inttypes_h.m4
  m4/lib-ld.m4
  m4/lib-link.m4
  m4/lib-prefix.m4
  m4/longdouble.m4
  m4/longlong.m4
  m4/mbchar.m4
  m4/mbiter.m4
  m4/mbrtowc.m4
  m4/md4.m4
  m4/md5.m4
  m4/memchr.m4
  m4/memxor.m4
  m4/mktime.m4
  m4/onceonly_2_57.m4
  m4/readlink.m4
  m4/restrict.m4
  m4/setenv.m4
  m4/sha1.m4
  m4/signed.m4
  m4/size_max.m4
  m4/socklen.m4
  m4/sockpfaf.m4
  m4/ssize_t.m4
  m4/stdarg.m4
  m4/stdbool.m4
  m4/stdint.m4
  m4/stdint_h.m4
  m4/strcase.m4
  m4/strchrnul.m4
  m4/strdup.m4
  m4/strerror_r.m4
  m4/strndup.m4
  m4/strnlen.m4
  m4/strtok_r.m4
  m4/strverscmp.m4
  m4/sys_socket_h.m4
  m4/time_r.m4
  m4/timegm.m4
  m4/timespec.m4
  m4/tm_gmtoff.m4
  m4/unistd_h.m4
  m4/vasnprintf.m4
  m4/vasprintf.m4
  m4/wchar_t.m4
  m4/wint_t.m4
  m4/xalloc.m4
  m4/xreadlink.m4
  m4/xsize.m4
  m4/xstrndup.m4
  m4/xvasprintf.m4
])
