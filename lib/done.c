/* done.c	deconstructor
 * Copyright (C) 2002  Simon Josefsson
 *
 * This file is part of Shishi.
 *
 * Shishi is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Shishi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Shishi; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "internal.h"

/**
 * shishi_done:
 * @handle: shishi handle as allocated by shishi_init().
 * 
 * Deallocates the shishi library handle.  The handle must not be used
 * in any calls to shishi functions after this.
 **/
void
shishi_done (Shishi * handle)
{
  if (handle->ticketset)
    shishi_ticketset_done (handle, handle->ticketset);

  /*  if (handle->default_realm)
     free (handle->default_realm); */
  if (handle->usercfgfile)
    free (handle->usercfgfile);
  if (handle->ticketsetfile)
    free (handle->ticketsetfile);

  if (handle->asn1)
    asn1_delete_structure (&handle->asn1);

  free (handle);
}
