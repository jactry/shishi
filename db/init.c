/* init.c --- Initialization functions for the Shisa library.
 * Copyright (C) 2002, 2003  Simon Josefsson
 *
 * This file is part of Shishi.
 *
 * Shishi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Shishi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Shishi; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "internal.h"

/**
 * shisa:
 *
 * Initializes the Shisa library.  If this function fails, it may
 * print diagnostic errors to stderr.
 *
 * Return value: Returns Shisa library handle, or %NULL on error.
 **/
Shisa *
shisa (void)
{
  Shisa *dbh;

  dbh = xcalloc (1, sizeof (*dbh));

  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  return dbh;
}

/**
 * shisa_done:
 * @handle: shisa handle as allocated by shisa().
 *
 * Deallocates the shisa library handle.  The handle must not be used
 * in any calls to shisa functions after this.
 **/
void
shisa_done (Shisa * dbh)
{
  free (dbh);
}

/**
 * shisa_init:
 * @dbh: pointer to library handle to be created.
 *
 * Create a Shisa library handle, using shisa(), and read the system
 * configuration file from their default locations.  The paths to the
 * default system configuration file is decided at compile time
 * ($sysconfdir/shisa.conf).
 *
 * The handle is allocated regardless of return values, except for
 * SHISA_HANDLE_ERROR which indicates a problem allocating the handle.
 * (The other error conditions comes from reading the files.)
 *
 * Return value: Returns %SHISA_OK iff successful.
 **/
int
shisa_init (Shisa ** dbh)
{
  return shisa_init_with_paths (dbh, NULL);
}

/**
 * shisa_init_with_paths:
 * @dbh: pointer to library handle to be created.
 * @file: Filename of system configuration, or NULL.

 * Create a Shisa library handle, using shisa(), and read the system
 * configuration file indicated location (or the default location, if
 * %NULL).  The paths to the default system configuration file is
 * decided at compile time ($sysconfdir/shisa.conf).
 *
 * The handle is allocated regardless of return values, except for
 * SHISA_HANDLE_ERROR which indicates a problem allocating the handle.
 * (The other error conditions comes from reading the files.)
 *
 * Return value: Returns %SHISA_OK iff successful.
 **/
int
shisa_init_with_paths (Shisa ** dbh, const char *file)
{
  if (!dbh || !(*dbh = shisa ()))
    return SHISA_HANDLE_ERROR;

  if (!file)
    file = shisa_cfg_default_systemfile (*dbh);

  return shisa_cfg_from_file (*dbh, file);
}
