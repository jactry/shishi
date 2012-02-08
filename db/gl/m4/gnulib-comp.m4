# DO NOT EDIT! GENERATED AUTOMATICALLY!
# Copyright (C) 2002-2012 Free Software Foundation, Inc.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <http://www.gnu.org/licenses/>.
#
# As a special exception to the GNU General Public License,
# this file may be distributed as part of a program that
# contains a configuration script generated by Autoconf, under
# the same distribution terms as the rest of that program.
#
# Generated by gnulib-tool.
#
# This file represents the compiled summary of the specification in
# gnulib-cache.m4. It lists the computed macro invocations that need
# to be invoked from configure.ac.
# In projects that use version control, this file can be treated like
# other built files.


# This macro should be invoked from ./configure.ac, in the section
# "Checks for programs", right after AC_PROG_CC, and certainly before
# any checks for libraries, header files, types and library functions.
AC_DEFUN([gl3_EARLY],
[
  m4_pattern_forbid([^gl_[A-Z]])dnl the gnulib macro namespace
  m4_pattern_allow([^gl_ES$])dnl a valid locale name
  m4_pattern_allow([^gl_LIBOBJS$])dnl a variable
  m4_pattern_allow([^gl_LTLIBOBJS$])dnl a variable
  AC_REQUIRE([gl_PROG_AR_RANLIB])
  # Code from module allocator:
  # Code from module areadlink:
  # Code from module careadlinkat:
  # Code from module readlink:
])

# This macro should be invoked from ./configure.ac, in the section
# "Check for header files, types and library functions".
AC_DEFUN([gl3_INIT],
[
  AM_CONDITIONAL([GL_COND_LIBTOOL], [true])
  gl_cond_libtool=true
  gl_m4_base='db/gl/m4'
  m4_pushdef([AC_LIBOBJ], m4_defn([gl3_LIBOBJ]))
  m4_pushdef([AC_REPLACE_FUNCS], m4_defn([gl3_REPLACE_FUNCS]))
  m4_pushdef([AC_LIBSOURCES], m4_defn([gl3_LIBSOURCES]))
  m4_pushdef([gl3_LIBSOURCES_LIST], [])
  m4_pushdef([gl3_LIBSOURCES_DIR], [])
  gl_COMMON
  gl_source_base='db/gl'
AC_CHECK_FUNCS_ONCE([readlinkat])
gl_FUNC_READLINK
if test $HAVE_READLINK = 0 || test $REPLACE_READLINK = 1; then
  AC_LIBOBJ([readlink])
  gl_PREREQ_READLINK
fi
gl_UNISTD_MODULE_INDICATOR([readlink])
  # End of code from modules
  m4_ifval(gl3_LIBSOURCES_LIST, [
    m4_syscmd([test ! -d ]m4_defn([gl3_LIBSOURCES_DIR])[ ||
      for gl_file in ]gl3_LIBSOURCES_LIST[ ; do
        if test ! -r ]m4_defn([gl3_LIBSOURCES_DIR])[/$gl_file ; then
          echo "missing file ]m4_defn([gl3_LIBSOURCES_DIR])[/$gl_file" >&2
          exit 1
        fi
      done])dnl
      m4_if(m4_sysval, [0], [],
        [AC_FATAL([expected source file, required through AC_LIBSOURCES, not found])])
  ])
  m4_popdef([gl3_LIBSOURCES_DIR])
  m4_popdef([gl3_LIBSOURCES_LIST])
  m4_popdef([AC_LIBSOURCES])
  m4_popdef([AC_REPLACE_FUNCS])
  m4_popdef([AC_LIBOBJ])
  AC_CONFIG_COMMANDS_PRE([
    gl3_libobjs=
    gl3_ltlibobjs=
    if test -n "$gl3_LIBOBJS"; then
      # Remove the extension.
      sed_drop_objext='s/\.o$//;s/\.obj$//'
      for i in `for i in $gl3_LIBOBJS; do echo "$i"; done | sed -e "$sed_drop_objext" | sort | uniq`; do
        gl3_libobjs="$gl3_libobjs $i.$ac_objext"
        gl3_ltlibobjs="$gl3_ltlibobjs $i.lo"
      done
    fi
    AC_SUBST([gl3_LIBOBJS], [$gl3_libobjs])
    AC_SUBST([gl3_LTLIBOBJS], [$gl3_ltlibobjs])
  ])
  gltests_libdeps=
  gltests_ltlibdeps=
  m4_pushdef([AC_LIBOBJ], m4_defn([gl3tests_LIBOBJ]))
  m4_pushdef([AC_REPLACE_FUNCS], m4_defn([gl3tests_REPLACE_FUNCS]))
  m4_pushdef([AC_LIBSOURCES], m4_defn([gl3tests_LIBSOURCES]))
  m4_pushdef([gl3tests_LIBSOURCES_LIST], [])
  m4_pushdef([gl3tests_LIBSOURCES_DIR], [])
  gl_COMMON
  gl_source_base='tests'
changequote(,)dnl
  gl3tests_WITNESS=IN_`echo "${PACKAGE-$PACKAGE_TARNAME}" | LC_ALL=C tr abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ | LC_ALL=C sed -e 's/[^A-Z0-9_]/_/g'`_GNULIB_TESTS
changequote([, ])dnl
  AC_SUBST([gl3tests_WITNESS])
  gl_module_indicator_condition=$gl3tests_WITNESS
  m4_pushdef([gl_MODULE_INDICATOR_CONDITION], [$gl_module_indicator_condition])
  m4_popdef([gl_MODULE_INDICATOR_CONDITION])
  m4_ifval(gl3tests_LIBSOURCES_LIST, [
    m4_syscmd([test ! -d ]m4_defn([gl3tests_LIBSOURCES_DIR])[ ||
      for gl_file in ]gl3tests_LIBSOURCES_LIST[ ; do
        if test ! -r ]m4_defn([gl3tests_LIBSOURCES_DIR])[/$gl_file ; then
          echo "missing file ]m4_defn([gl3tests_LIBSOURCES_DIR])[/$gl_file" >&2
          exit 1
        fi
      done])dnl
      m4_if(m4_sysval, [0], [],
        [AC_FATAL([expected source file, required through AC_LIBSOURCES, not found])])
  ])
  m4_popdef([gl3tests_LIBSOURCES_DIR])
  m4_popdef([gl3tests_LIBSOURCES_LIST])
  m4_popdef([AC_LIBSOURCES])
  m4_popdef([AC_REPLACE_FUNCS])
  m4_popdef([AC_LIBOBJ])
  AC_CONFIG_COMMANDS_PRE([
    gl3tests_libobjs=
    gl3tests_ltlibobjs=
    if test -n "$gl3tests_LIBOBJS"; then
      # Remove the extension.
      sed_drop_objext='s/\.o$//;s/\.obj$//'
      for i in `for i in $gl3tests_LIBOBJS; do echo "$i"; done | sed -e "$sed_drop_objext" | sort | uniq`; do
        gl3tests_libobjs="$gl3tests_libobjs $i.$ac_objext"
        gl3tests_ltlibobjs="$gl3tests_ltlibobjs $i.lo"
      done
    fi
    AC_SUBST([gl3tests_LIBOBJS], [$gl3tests_libobjs])
    AC_SUBST([gl3tests_LTLIBOBJS], [$gl3tests_ltlibobjs])
  ])
])

# Like AC_LIBOBJ, except that the module name goes
# into gl3_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl3_LIBOBJ], [
  AS_LITERAL_IF([$1], [gl3_LIBSOURCES([$1.c])])dnl
  gl3_LIBOBJS="$gl3_LIBOBJS $1.$ac_objext"
])

# Like AC_REPLACE_FUNCS, except that the module name goes
# into gl3_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl3_REPLACE_FUNCS], [
  m4_foreach_w([gl_NAME], [$1], [AC_LIBSOURCES(gl_NAME[.c])])dnl
  AC_CHECK_FUNCS([$1], , [gl3_LIBOBJ($ac_func)])
])

# Like AC_LIBSOURCES, except the directory where the source file is
# expected is derived from the gnulib-tool parameterization,
# and alloca is special cased (for the alloca-opt module).
# We could also entirely rely on EXTRA_lib..._SOURCES.
AC_DEFUN([gl3_LIBSOURCES], [
  m4_foreach([_gl_NAME], [$1], [
    m4_if(_gl_NAME, [alloca.c], [], [
      m4_define([gl3_LIBSOURCES_DIR], [db/gl])
      m4_append([gl3_LIBSOURCES_LIST], _gl_NAME, [ ])
    ])
  ])
])

# Like AC_LIBOBJ, except that the module name goes
# into gl3tests_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl3tests_LIBOBJ], [
  AS_LITERAL_IF([$1], [gl3tests_LIBSOURCES([$1.c])])dnl
  gl3tests_LIBOBJS="$gl3tests_LIBOBJS $1.$ac_objext"
])

# Like AC_REPLACE_FUNCS, except that the module name goes
# into gl3tests_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([gl3tests_REPLACE_FUNCS], [
  m4_foreach_w([gl_NAME], [$1], [AC_LIBSOURCES(gl_NAME[.c])])dnl
  AC_CHECK_FUNCS([$1], , [gl3tests_LIBOBJ($ac_func)])
])

# Like AC_LIBSOURCES, except the directory where the source file is
# expected is derived from the gnulib-tool parameterization,
# and alloca is special cased (for the alloca-opt module).
# We could also entirely rely on EXTRA_lib..._SOURCES.
AC_DEFUN([gl3tests_LIBSOURCES], [
  m4_foreach([_gl_NAME], [$1], [
    m4_if(_gl_NAME, [alloca.c], [], [
      m4_define([gl3tests_LIBSOURCES_DIR], [tests])
      m4_append([gl3tests_LIBSOURCES_LIST], _gl_NAME, [ ])
    ])
  ])
])

# This macro records the list of files which have been installed by
# gnulib-tool and may be removed by future gnulib-tool invocations.
AC_DEFUN([gl3_FILE_LIST], [
  lib/allocator.c
  lib/allocator.h
  lib/areadlink.c
  lib/areadlink.h
  lib/careadlinkat.c
  lib/careadlinkat.h
  lib/readlink.c
  m4/00gnulib.m4
  m4/gnulib-common.m4
  m4/readlink.m4
])
