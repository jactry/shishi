/* Duplicate N bytes of data buffer, with out-of-memory checking.
   Copyright (C) 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#if HAVE_CONFIG_H
# include <config.h>
#endif

/* Specification.  */
#include "xmemdup.h"

/* Get xmalloc */
#include "xalloc.h"

/* Duplicate N bytes of data buffer, with out-of-memory checking. */
extern void *
xmemdup (void *dest, const void *src, size_t n)
{
  dest = xmalloc (n);
  memcpy (dest, src, n);
  return dest;
}