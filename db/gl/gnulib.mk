## DO NOT EDIT! GENERATED AUTOMATICALLY!
## Process this file with automake to produce Makefile.in.
# Copyright (C) 2002-2014 Free Software Foundation, Inc.
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
# Reproduce by: gnulib-tool --import --dir=. --lib=libgnu --source-base=db/gl --m4-base=db/gl/m4 --doc-base=doc --tests-base=tests --aux-dir=build-aux --avoid=absolute-header --avoid=include_next --avoid=link-warning --avoid=malloc-posix --avoid=ssize_t --avoid=stat --avoid=stdlib --avoid=unistd --makefile-name=gnulib.mk --no-conditional-dependencies --libtool --macro-prefix=gl3 --no-vc-files areadlink


MOSTLYCLEANFILES += core *.stackdump

noinst_LTLIBRARIES += libgnu.la

libgnu_la_SOURCES =
libgnu_la_LIBADD = $(gl3_LTLIBOBJS)
libgnu_la_DEPENDENCIES = $(gl3_LTLIBOBJS)
EXTRA_libgnu_la_SOURCES =
libgnu_la_LDFLAGS = $(AM_LDFLAGS)
libgnu_la_LDFLAGS += -no-undefined

## begin gnulib module allocator

libgnu_la_SOURCES += allocator.c

EXTRA_DIST += allocator.h

## end   gnulib module allocator

## begin gnulib module areadlink

libgnu_la_SOURCES += areadlink.c

EXTRA_DIST += areadlink.h

## end   gnulib module areadlink

## begin gnulib module careadlinkat

libgnu_la_SOURCES += careadlinkat.c

EXTRA_DIST += careadlinkat.h

## end   gnulib module careadlinkat

## begin gnulib module readlink


EXTRA_DIST += readlink.c

EXTRA_libgnu_la_SOURCES += readlink.c

## end   gnulib module readlink


mostlyclean-local: mostlyclean-generic
	@for dir in '' $(MOSTLYCLEANDIRS); do \
	  if test -n "$$dir" && test -d $$dir; then \
	    echo "rmdir $$dir"; rmdir $$dir; \
	  fi; \
	done; \
	:
