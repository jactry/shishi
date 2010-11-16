## DO NOT EDIT! GENERATED AUTOMATICALLY!
## Process this file with automake to produce Makefile.in.
# Copyright (C) 2002-2010 Free Software Foundation, Inc.
#
# This file is free software, distributed under the terms of the GNU
# General Public License.  As a special exception to the GNU General
# Public License, this file may be distributed as part of a program
# that contains a configuration script generated by Autoconf, under
# the same distribution terms as the rest of that program.
#
# Generated by gnulib-tool.
# Reproduce by: gnulib-tool --import --dir=. --lib=libgnu --source-base=db/gl --m4-base=db/gl/m4 --doc-base=doc --tests-base=tests --aux-dir=build-aux --avoid=absolute-header --avoid=include_next --avoid=link-warning --avoid=malloc-posix --avoid=ssize_t --avoid=stat --avoid=stdlib --avoid=unistd --makefile-name=gnulib.mk --libtool --macro-prefix=gl3 --no-vc-files areadlink


MOSTLYCLEANFILES += core *.stackdump

noinst_LTLIBRARIES += libgnu.la

libgnu_la_SOURCES =
libgnu_la_LIBADD = $(gl3_LTLIBOBJS)
libgnu_la_DEPENDENCIES = $(gl3_LTLIBOBJS)
EXTRA_libgnu_la_SOURCES =
libgnu_la_LDFLAGS = $(AM_LDFLAGS)
libgnu_la_LDFLAGS += -no-undefined

## begin gnulib module areadlink

libgnu_la_SOURCES += areadlink.c

EXTRA_DIST += areadlink.h

## end   gnulib module areadlink

## begin gnulib module arg-nonnull

# The BUILT_SOURCES created by this Makefile snippet are not used via #include
# statements but through direct file reference. Therefore this snippet must be
# present in all Makefile.am that need it. This is ensured by the applicability
# 'all' defined above.

BUILT_SOURCES += arg-nonnull.h
# The arg-nonnull.h that gets inserted into generated .h files is the same as
# build-aux/arg-nonnull.h, except that it has the copyright header cut off.
arg-nonnull.h: $(top_srcdir)/build-aux/arg-nonnull.h
	$(AM_V_GEN)rm -f $@-t $@ && \
	sed -n -e '/GL_ARG_NONNULL/,$$p' \
	  < $(top_srcdir)/build-aux/arg-nonnull.h \
	  > $@-t && \
	mv $@-t $@
MOSTLYCLEANFILES += arg-nonnull.h arg-nonnull.h-t

ARG_NONNULL_H=arg-nonnull.h

EXTRA_DIST += $(top_srcdir)/build-aux/arg-nonnull.h

## end   gnulib module arg-nonnull

## begin gnulib module c++defs

# The BUILT_SOURCES created by this Makefile snippet are not used via #include
# statements but through direct file reference. Therefore this snippet must be
# present in all Makefile.am that need it. This is ensured by the applicability
# 'all' defined above.

BUILT_SOURCES += c++defs.h
# The c++defs.h that gets inserted into generated .h files is the same as
# build-aux/c++defs.h, except that it has the copyright header cut off.
c++defs.h: $(top_srcdir)/build-aux/c++defs.h
	$(AM_V_GEN)rm -f $@-t $@ && \
	sed -n -e '/_GL_CXXDEFS/,$$p' \
	  < $(top_srcdir)/build-aux/c++defs.h \
	  > $@-t && \
	mv $@-t $@
MOSTLYCLEANFILES += c++defs.h c++defs.h-t

CXXDEFS_H=c++defs.h

EXTRA_DIST += $(top_srcdir)/build-aux/c++defs.h

## end   gnulib module c++defs

## begin gnulib module readlink


EXTRA_DIST += readlink.c

EXTRA_libgnu_la_SOURCES += readlink.c

## end   gnulib module readlink

## begin gnulib module stddef

BUILT_SOURCES += $(STDDEF_H)

# We need the following in order to create <stddef.h> when the system
# doesn't have one that works with the given compiler.
stddef.h: stddef.in.h
	$(AM_V_GEN)rm -f $@-t $@ && \
	{ echo '/* DO NOT EDIT! GENERATED AUTOMATICALLY! */' && \
	  sed -e 's|@''INCLUDE_NEXT''@|$(INCLUDE_NEXT)|g' \
	      -e 's|@''PRAGMA_SYSTEM_HEADER''@|@PRAGMA_SYSTEM_HEADER@|g' \
	      -e 's|@''PRAGMA_COLUMNS''@|@PRAGMA_COLUMNS@|g' \
	      -e 's|@''NEXT_STDDEF_H''@|$(NEXT_STDDEF_H)|g' \
	      -e 's|@''HAVE_WCHAR_T''@|$(HAVE_WCHAR_T)|g' \
	      -e 's|@''REPLACE_NULL''@|$(REPLACE_NULL)|g' \
	      < $(srcdir)/stddef.in.h; \
	} > $@-t && \
	mv $@-t $@
MOSTLYCLEANFILES += stddef.h stddef.h-t

EXTRA_DIST += stddef.in.h

## end   gnulib module stddef

## begin gnulib module stdint

BUILT_SOURCES += $(STDINT_H)

# We need the following in order to create <stdint.h> when the system
# doesn't have one that works with the given compiler.
stdint.h: stdint.in.h
	$(AM_V_GEN)rm -f $@-t $@ && \
	{ echo '/* DO NOT EDIT! GENERATED AUTOMATICALLY! */'; \
	  sed -e 's/@''HAVE_STDINT_H''@/$(HAVE_STDINT_H)/g' \
	      -e 's|@''INCLUDE_NEXT''@|$(INCLUDE_NEXT)|g' \
	      -e 's|@''PRAGMA_SYSTEM_HEADER''@|@PRAGMA_SYSTEM_HEADER@|g' \
	      -e 's|@''PRAGMA_COLUMNS''@|@PRAGMA_COLUMNS@|g' \
	      -e 's|@''NEXT_STDINT_H''@|$(NEXT_STDINT_H)|g' \
	      -e 's/@''HAVE_SYS_TYPES_H''@/$(HAVE_SYS_TYPES_H)/g' \
	      -e 's/@''HAVE_INTTYPES_H''@/$(HAVE_INTTYPES_H)/g' \
	      -e 's/@''HAVE_SYS_INTTYPES_H''@/$(HAVE_SYS_INTTYPES_H)/g' \
	      -e 's/@''HAVE_SYS_BITYPES_H''@/$(HAVE_SYS_BITYPES_H)/g' \
	      -e 's/@''HAVE_LONG_LONG_INT''@/$(HAVE_LONG_LONG_INT)/g' \
	      -e 's/@''HAVE_UNSIGNED_LONG_LONG_INT''@/$(HAVE_UNSIGNED_LONG_LONG_INT)/g' \
	      -e 's/@''APPLE_UNIVERSAL_BUILD''@/$(APPLE_UNIVERSAL_BUILD)/g' \
	      -e 's/@''BITSIZEOF_PTRDIFF_T''@/$(BITSIZEOF_PTRDIFF_T)/g' \
	      -e 's/@''PTRDIFF_T_SUFFIX''@/$(PTRDIFF_T_SUFFIX)/g' \
	      -e 's/@''BITSIZEOF_SIG_ATOMIC_T''@/$(BITSIZEOF_SIG_ATOMIC_T)/g' \
	      -e 's/@''HAVE_SIGNED_SIG_ATOMIC_T''@/$(HAVE_SIGNED_SIG_ATOMIC_T)/g' \
	      -e 's/@''SIG_ATOMIC_T_SUFFIX''@/$(SIG_ATOMIC_T_SUFFIX)/g' \
	      -e 's/@''BITSIZEOF_SIZE_T''@/$(BITSIZEOF_SIZE_T)/g' \
	      -e 's/@''SIZE_T_SUFFIX''@/$(SIZE_T_SUFFIX)/g' \
	      -e 's/@''BITSIZEOF_WCHAR_T''@/$(BITSIZEOF_WCHAR_T)/g' \
	      -e 's/@''HAVE_SIGNED_WCHAR_T''@/$(HAVE_SIGNED_WCHAR_T)/g' \
	      -e 's/@''WCHAR_T_SUFFIX''@/$(WCHAR_T_SUFFIX)/g' \
	      -e 's/@''BITSIZEOF_WINT_T''@/$(BITSIZEOF_WINT_T)/g' \
	      -e 's/@''HAVE_SIGNED_WINT_T''@/$(HAVE_SIGNED_WINT_T)/g' \
	      -e 's/@''WINT_T_SUFFIX''@/$(WINT_T_SUFFIX)/g' \
	      < $(srcdir)/stdint.in.h; \
	} > $@-t && \
	mv $@-t $@
MOSTLYCLEANFILES += stdint.h stdint.h-t

EXTRA_DIST += stdint.in.h

## end   gnulib module stdint

## begin gnulib module warn-on-use

BUILT_SOURCES += warn-on-use.h
# The warn-on-use.h that gets inserted into generated .h files is the same as
# build-aux/warn-on-use.h, except that it has the copyright header cut off.
warn-on-use.h: $(top_srcdir)/build-aux/warn-on-use.h
	$(AM_V_GEN)rm -f $@-t $@ && \
	sed -n -e '/^.ifndef/,$$p' \
	  < $(top_srcdir)/build-aux/warn-on-use.h \
	  > $@-t && \
	mv $@-t $@
MOSTLYCLEANFILES += warn-on-use.h warn-on-use.h-t

WARN_ON_USE_H=warn-on-use.h

EXTRA_DIST += $(top_srcdir)/build-aux/warn-on-use.h

## end   gnulib module warn-on-use

## begin gnulib module wchar

BUILT_SOURCES += wchar.h

# We need the following in order to create <wchar.h> when the system
# version does not work standalone.
wchar.h: wchar.in.h $(CXXDEFS_H) $(ARG_NONNULL_H) $(WARN_ON_USE_H)
	$(AM_V_GEN)rm -f $@-t $@ && \
	{ echo '/* DO NOT EDIT! GENERATED AUTOMATICALLY! */'; \
	  sed -e 's|@''INCLUDE_NEXT''@|$(INCLUDE_NEXT)|g' \
	      -e 's|@''PRAGMA_SYSTEM_HEADER''@|@PRAGMA_SYSTEM_HEADER@|g' \
	      -e 's|@''PRAGMA_COLUMNS''@|@PRAGMA_COLUMNS@|g' \
	      -e 's|@''NEXT_WCHAR_H''@|$(NEXT_WCHAR_H)|g' \
	      -e 's|@''HAVE_WCHAR_H''@|$(HAVE_WCHAR_H)|g' \
	      -e 's|@''GNULIB_BTOWC''@|$(GNULIB_BTOWC)|g' \
	      -e 's|@''GNULIB_WCTOB''@|$(GNULIB_WCTOB)|g' \
	      -e 's|@''GNULIB_MBSINIT''@|$(GNULIB_MBSINIT)|g' \
	      -e 's|@''GNULIB_MBRTOWC''@|$(GNULIB_MBRTOWC)|g' \
	      -e 's|@''GNULIB_MBRLEN''@|$(GNULIB_MBRLEN)|g' \
	      -e 's|@''GNULIB_MBSRTOWCS''@|$(GNULIB_MBSRTOWCS)|g' \
	      -e 's|@''GNULIB_MBSNRTOWCS''@|$(GNULIB_MBSNRTOWCS)|g' \
	      -e 's|@''GNULIB_WCRTOMB''@|$(GNULIB_WCRTOMB)|g' \
	      -e 's|@''GNULIB_WCSRTOMBS''@|$(GNULIB_WCSRTOMBS)|g' \
	      -e 's|@''GNULIB_WCSNRTOMBS''@|$(GNULIB_WCSNRTOMBS)|g' \
	      -e 's|@''GNULIB_WCWIDTH''@|$(GNULIB_WCWIDTH)|g' \
	      -e 's|@''HAVE_WINT_T''@|$(HAVE_WINT_T)|g' \
	      -e 's|@''HAVE_BTOWC''@|$(HAVE_BTOWC)|g' \
	      -e 's|@''HAVE_MBSINIT''@|$(HAVE_MBSINIT)|g' \
	      -e 's|@''HAVE_MBRTOWC''@|$(HAVE_MBRTOWC)|g' \
	      -e 's|@''HAVE_MBRLEN''@|$(HAVE_MBRLEN)|g' \
	      -e 's|@''HAVE_MBSRTOWCS''@|$(HAVE_MBSRTOWCS)|g' \
	      -e 's|@''HAVE_MBSNRTOWCS''@|$(HAVE_MBSNRTOWCS)|g' \
	      -e 's|@''HAVE_WCRTOMB''@|$(HAVE_WCRTOMB)|g' \
	      -e 's|@''HAVE_WCSRTOMBS''@|$(HAVE_WCSRTOMBS)|g' \
	      -e 's|@''HAVE_WCSNRTOMBS''@|$(HAVE_WCSNRTOMBS)|g' \
	      -e 's|@''HAVE_DECL_WCTOB''@|$(HAVE_DECL_WCTOB)|g' \
	      -e 's|@''HAVE_DECL_WCWIDTH''@|$(HAVE_DECL_WCWIDTH)|g' \
	      -e 's|@''REPLACE_MBSTATE_T''@|$(REPLACE_MBSTATE_T)|g' \
	      -e 's|@''REPLACE_BTOWC''@|$(REPLACE_BTOWC)|g' \
	      -e 's|@''REPLACE_WCTOB''@|$(REPLACE_WCTOB)|g' \
	      -e 's|@''REPLACE_MBSINIT''@|$(REPLACE_MBSINIT)|g' \
	      -e 's|@''REPLACE_MBRTOWC''@|$(REPLACE_MBRTOWC)|g' \
	      -e 's|@''REPLACE_MBRLEN''@|$(REPLACE_MBRLEN)|g' \
	      -e 's|@''REPLACE_MBSRTOWCS''@|$(REPLACE_MBSRTOWCS)|g' \
	      -e 's|@''REPLACE_MBSNRTOWCS''@|$(REPLACE_MBSNRTOWCS)|g' \
	      -e 's|@''REPLACE_WCRTOMB''@|$(REPLACE_WCRTOMB)|g' \
	      -e 's|@''REPLACE_WCSRTOMBS''@|$(REPLACE_WCSRTOMBS)|g' \
	      -e 's|@''REPLACE_WCSNRTOMBS''@|$(REPLACE_WCSNRTOMBS)|g' \
	      -e 's|@''REPLACE_WCWIDTH''@|$(REPLACE_WCWIDTH)|g' \
	      -e '/definitions of _GL_FUNCDECL_RPL/r $(CXXDEFS_H)' \
	      -e '/definition of _GL_ARG_NONNULL/r $(ARG_NONNULL_H)' \
	      -e '/definition of _GL_WARN_ON_USE/r $(WARN_ON_USE_H)' \
	    < $(srcdir)/wchar.in.h; \
	} > $@-t && \
	mv $@-t $@
MOSTLYCLEANFILES += wchar.h wchar.h-t

EXTRA_DIST += wchar.in.h

## end   gnulib module wchar


mostlyclean-local: mostlyclean-generic
	@for dir in '' $(MOSTLYCLEANDIRS); do \
	  if test -n "$$dir" && test -d $$dir; then \
	    echo "rmdir $$dir"; rmdir $$dir; \
	  fi; \
	done; \
	:
