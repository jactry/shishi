/* strerror.c - Describing an error code.
   Copyright (C) 2003 g10 Code GmbH

   This file is part of libgpg-error.

   libgpg-error is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
 
   libgpg-error is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public
   License along with libgpg-error; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <gpg-error.h>

#include "gettext.h"
#include "err-codes.h"

/* Return a pointer to a string containing a description of the error
   code in the error value ERR.  */
const char *
gpg_strerror (gpg_error_t err)
{
  gpg_err_code_t code = gpg_err_code (err);

  if (code & GPG_ERR_SYSTEM_ERROR)
    {
      int no = gpg_err_code_to_errno (code);
      if (no)
	return strerror (no);
      else
	code = GPG_ERR_UNKNOWN_ERRNO;
    }
  return dgettext (PACKAGE, msgstr + msgidx[msgidxof (code)]);
}
