# DO NOT EDIT! GENERATED AUTOMATICALLY!
# Copyright (C) 2004-2007 Free Software Foundation, Inc.
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
AC_DEFUN([gl2_EARLY],
[
  m4_pattern_forbid([^gl_[A-Z]])dnl the gnulib macro namespace
  m4_pattern_allow([^gl_ES$])dnl a valid locale name
  m4_pattern_allow([^gl_LIBOBJS$])dnl a variable
  m4_pattern_allow([^gl_LTLIBOBJS$])dnl a variable
  AC_REQUIRE([AC_PROG_RANLIB])
])

# This macro should be invoked from ./configure.ac, in the section
# "Check for header files, types and library functions".
AC_DEFUN([gl2_INIT],
[
  AM_CONDITIONAL([GL_COND_LIBTOOL], [true])
  gl_cond_libtool=true
  m4_pushdef([AC_LIBOBJ], m4_defn([gl2_LIBOBJ]))
  m4_pushdef([AC_REPLACE_FUNCS], m4_defn([gl2_REPLACE_FUNCS]))
  m4_pushdef([AC_LIBSOURCES], m4_defn([gl2_LIBSOURCES]))
  gl_source_base='src/gl'
  gl_ERROR
  m4_ifdef([AM_XGETTEXT_OPTION],
    [AM_XGETTEXT_OPTION([--flag=error:3:c-format])
     AM_XGETTEXT_OPTION([--flag=error_at_line:5:c-format])])
  gl_LOCALE_H
  gl_FUNC_STRERROR
  gl_STRING_MODULE_INDICATOR([strerror])
  m4_popdef([AC_LIBSOURCES])
  m4_popdef([AC_REPLACE_FUNCS])
  m4_popdef([AC_LIBOBJ])
  AC_CONFIG_COMMANDS_PRE([
    gl2_libobjs=
    gl2_ltlibobjs=
    if test -n "$gl2_LIBOBJS"; then
      # Remove the extension.
      sed_drop_objext='s/\.o$//;s/\.obj$//'
      for i in `for i in $gl2_LIBOBJS; do echo "$i"; done | sed "$sed_drop_objext" | sort | uniq`; do
        gl2_libobjs="$gl2_libobjs $i.$ac_objext"
        gl2_ltlibobjs="$gl2_ltlibobjs $i.lo"
      done
    fi
    AC_SUBST([gl2_LIBOBJS], [$gl2_libobjs])
    AC_SUBST([gl2_LTLIBOBJS], [$gl2_ltlibobjs])
  ])
  gltests_libdeps=
  gltests_ltlibdeps=
  m4_pushdef([AC_LIBOBJ], m4_defn([gl2tests_LIBOBJ]))
  m4_pushdef([AC_REPLACE_FUNCS], m4_defn([gl2tests_REPLACE_FUNCS]))
  m4_pushdef([AC_LIBSOURCES], m4_defn([gl2tests_LIBSOURCES]))
  gl_source_base='tests'
  m4_popdef([AC_LIBSOURCES])
  m4_popdef([AC_REPLACE_FUNCS])
  m4_popdef([AC_LIBOBJ])
  AC_CONFIG_COMMANDS_PRE([
    gl2tests_libobjs=
    gl2tests_ltlibobjs=
    if test -n "$gl2tests_LIBOBJS"; then
      # Remove the extension.
      sed_drop_objext='s/\.o$//;s/\.obj$//'
      for i in `for i in $gl2tests_LIBOBJS; do echo "$i"; done | sed "$sed_drop_objext" | sort | uniq`; do
        gl2tests_libobjs="$gl2tests_libobjs $i.$ac_objext"
        gl2tests_ltlibobjs="$gl2tests_ltlibobjs $i.lo"
      done
    fi
    AC_SUBST([gl2tests_LIBOBJS], [$gl2tests_libobjs])
    AC_SUBST([gl2tests_LTLIBOBJS], [$gl2tests_ltlibobjs])
  ])
])

# Like AC_LIBOBJ, except that the module name goes
# into gl2_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl2_LIBOBJ], [
  AS_LITERAL_IF([$1], [gl2_LIBSOURCES([$1.c])])dnl
  gl2_LIBOBJS="$gl2_LIBOBJS $1.$ac_objext"
])

# m4_foreach_w is provided by autoconf-2.59c and later.
# This definition is to accommodate developers using versions
# of autoconf older than that.
m4_ifndef([m4_foreach_w],
  [m4_define([m4_foreach_w],
    [m4_foreach([$1], m4_split(m4_normalize([$2]), [ ]), [$3])])])

# Like AC_REPLACE_FUNCS, except that the module name goes
# into gl2_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl2_REPLACE_FUNCS], [
  m4_foreach_w([gl_NAME], [$1], [AC_LIBSOURCES(gl_NAME[.c])])dnl
  AC_CHECK_FUNCS([$1], , [gl2_LIBOBJ($ac_func)])
])

# Like AC_LIBSOURCES, except the directory where the source file is
# expected is derived from the gnulib-tool parametrization,
# and alloca is special cased (for the alloca-opt module).
# We could also entirely rely on EXTRA_lib..._SOURCES.
AC_DEFUN([gl2_LIBSOURCES], [
  m4_foreach([_gl_NAME], [$1], [
    m4_if(_gl_NAME, [alloca.c], [], [
      m4_syscmd([test -r src/gl/]_gl_NAME[ || test ! -d src/gl])dnl
      m4_if(m4_sysval, [0], [],
        [AC_FATAL([missing src/gl/]_gl_NAME)])
    ])
  ])
])

# Like AC_LIBOBJ, except that the module name goes
# into gl2tests_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl2tests_LIBOBJ], [
  AS_LITERAL_IF([$1], [gl2tests_LIBSOURCES([$1.c])])dnl
  gl2tests_LIBOBJS="$gl2tests_LIBOBJS $1.$ac_objext"
])

# m4_foreach_w is provided by autoconf-2.59c and later.
# This definition is to accommodate developers using versions
# of autoconf older than that.
m4_ifndef([m4_foreach_w],
  [m4_define([m4_foreach_w],
    [m4_foreach([$1], m4_split(m4_normalize([$2]), [ ]), [$3])])])

# Like AC_REPLACE_FUNCS, except that the module name goes
# into gl2tests_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl2tests_REPLACE_FUNCS], [
  m4_foreach_w([gl_NAME], [$1], [AC_LIBSOURCES(gl_NAME[.c])])dnl
  AC_CHECK_FUNCS([$1], , [gl2tests_LIBOBJ($ac_func)])
])

# Like AC_LIBSOURCES, except the directory where the source file is
# expected is derived from the gnulib-tool parametrization,
# and alloca is special cased (for the alloca-opt module).
# We could also entirely rely on EXTRA_lib..._SOURCES.
AC_DEFUN([gl2tests_LIBSOURCES], [
  m4_foreach([_gl_NAME], [$1], [
    m4_if(_gl_NAME, [alloca.c], [], [
      m4_syscmd([test -r tests/]_gl_NAME[ || test ! -d tests])dnl
      m4_if(m4_sysval, [0], [],
        [AC_FATAL([missing tests/]_gl_NAME)])
    ])
  ])
])

# This macro records the list of files which have been installed by
# gnulib-tool and may be removed by future gnulib-tool invocations.
AC_DEFUN([gl2_FILE_LIST], [
  lib/error.c
  lib/error.h
  lib/locale.in.h
  lib/progname.c
  lib/progname.h
  lib/strerror.c
  lib/version-etc.c
  lib/version-etc.h
  m4/error.m4
  m4/gnulib-common.m4
  m4/locale_h.m4
  m4/strerror.m4
])
