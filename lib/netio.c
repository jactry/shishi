/* netio.c	network I/O functions
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

static int
shishi_sendrecv_udp (Shishi * handle,
		     struct sockaddr *addr,
		     const char *indata, int inlen,
		     char *outdata, int *outlen, int timeout)
{
  struct sockaddr lsa;
  struct sockaddr_in *lsa_inp = (struct sockaddr_in *) &lsa;
  int sockfd;
  int bytes_sent;
  struct sockaddr_storage from_sa;
  int length = sizeof (struct sockaddr_storage);
  fd_set readfds;
  struct timeval tout;
  int rc;

  memset (&lsa, 0, sizeof (lsa));
  lsa_inp->sin_family = AF_INET;
  lsa_inp->sin_addr.s_addr = htonl (INADDR_ANY);

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
    {
      shishi_error_set (handle, strerror (errno));
      return SHISHI_SOCKET_ERROR;
    }

  if (bind (sockfd, (struct sockaddr *) &lsa, sizeof (lsa)) != 0)
    {
      shishi_error_set (handle, strerror (errno));
      close (sockfd);
      return SHISHI_BIND_ERROR;
    }

  bytes_sent = sendto (sockfd, (const void *) indata, inlen,
		       0, addr, sizeof (*addr));
  if (bytes_sent != inlen)
    {
      shishi_error_set (handle, strerror (errno));
      return SHISHI_SENDTO_ERROR;
    }

  FD_ZERO (&readfds);
  FD_SET (sockfd, &readfds);
  tout.tv_sec = timeout;
  tout.tv_usec = 0;
  if ((rc = select (sockfd + 1, &readfds, NULL, NULL, &tout)) != 1)
    {
      if (rc == -1)
	shishi_error_set (handle, strerror (errno));
      else
	shishi_error_clear (handle);
      return SHISHI_KDC_TIMEOUT;
    }

  *outlen = recvfrom (sockfd, outdata, *outlen, 0,
		      (struct sockaddr *) &from_sa, &length);

  if (*outlen == -1)
    {
      shishi_error_set (handle, strerror (errno));
      return SHISHI_RECVFROM_ERROR;
    }

  if (close (sockfd) != 0)
    {
      shishi_error_set (handle, strerror (errno));
      return SHISHI_CLOSE_ERROR;
    }

  return SHISHI_OK;
}

static int
shishi_kdc_sendrecv_static (Shishi * handle, char *realm,
			    const char *indata, size_t inlen,
			    char *outdata, size_t * outlen)
{
  int i, j, k;
  int rc;

  for (i = 0; i < handle->nrealminfos; i++)
    if (realm && strcmp (handle->realminfos[i].name, realm) == 0)
      {
	for (j = 0; j < handle->kdcretries; j++)
	  for (k = 0; k < handle->realminfos[i].nkdcaddresses; k++)
	    {
	      struct Shishi_kdcinfo *ki =
		&handle->realminfos[i].kdcaddresses[k];

	      if (VERBOSE (handle))
		{
		  printf ("Sending to %s (%s)...\n", ki->name,
			  inet_ntoa (((struct sockaddr_in *)
				      &ki->sockaddress)->sin_addr));
		}

	      rc = shishi_sendrecv_udp (handle, &ki->sockaddress,
					indata, inlen, outdata, outlen,
					handle->kdctimeout);
	      if (rc != SHISHI_KDC_TIMEOUT)
		return rc;
	    }

	shishi_error_clear (handle);
	return SHISHI_KDC_TIMEOUT;
      }

  shishi_error_printf (handle, "No KDC defined for realm %s", realm);
  return SHISHI_KDC_NOT_KNOWN_FOR_REALM;
}

static int
shishi_kdc_sendrecv_srv_1 (Shishi * handle, char *realm,
			   const char *indata, size_t inlen,
			   char *outdata, size_t * outlen,
			   dnshost_t rrs)
{
  int rc;

  for (; rrs; rrs = rrs->next)
    {
      dns_srv_t srv = (dns_srv_t) rrs->rr;
      struct addrinfo hints;
      struct addrinfo *ai;
      char *port;

      if (rrs->class != C_IN)
	continue;
      if (rrs->type != T_SRV)
	continue;

      if (VERBOSE (handle))
	printf ("Located SRV RRs server %s:%d...\n", srv->name, srv->port);

      memset (&hints, 0, sizeof(hints));
      hints.ai_socktype = SOCK_DGRAM;
      asprintf (&port, "%d", srv->port);
      rc = getaddrinfo (srv->name, port, &hints, &ai);
      free (port);

      if (rc != 0)
	{
	  shishi_warn (handle, "Unknown KDC host `%s' (gai rc %d)",
		       srv->name, rc);
	  freeaddrinfo (ai);
	  continue;
	}

      if (VERBOSE (handle))
	printf ("Sending to %s:%d (%s)...\n", srv->name, srv->port,
		inet_ntoa (((struct sockaddr_in *)ai->ai_addr)->sin_addr));

      rc = shishi_sendrecv_udp (handle, ai->ai_addr,
				indata, inlen, outdata, outlen,
				handle->kdctimeout);

      freeaddrinfo (ai);

      if (rc != SHISHI_KDC_TIMEOUT)
	return rc;
    }

  return SHISHI_KDC_TIMEOUT;
}

static int
shishi_kdc_sendrecv_srv (Shishi * handle, char *realm,
			 const char *indata, size_t inlen,
			 char *outdata, size_t * outlen)
{
  dnshost_t rrs;
  char *tmp;
  int rc;

  if (VERBOSE (handle))
    printf ("Finding SRV RRs for %s...\n", realm);

  asprintf (&tmp, "_kerberos._udp.%s", realm);
  rrs = _shishi_resolv(tmp, T_SRV);
  free (tmp);

  if (rrs)
    rc = shishi_kdc_sendrecv_srv_1 (handle, realm, indata, inlen,
				    outdata, outlen, rrs);
  else
    {
      shishi_error_printf (handle, "No KDC SRV RRs for realm %s\n", realm);
      rc = SHISHI_KDC_NOT_KNOWN_FOR_REALM;
    }

  _shishi_resolv_free (rrs);

  return rc;
}

static int
shishi_kdc_sendrecv_direct (Shishi * handle, char *realm,
			    const char *indata, size_t inlen,
			    char *outdata, size_t * outlen)
{
  struct servent *se;
  struct addrinfo hints;
  struct addrinfo *ai;
  char *port;
  int rc;

  se = getservbyname ("kerberos", NULL);
  if (se)
    asprintf (&port, "%d", ntohs(se->s_port));
  else
    asprintf (&port, "%d", 88);

  memset (&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_DGRAM;
  rc = getaddrinfo (realm, port, &hints, &ai);

  free (port);

  if (rc != 0)
    {
      shishi_warn (handle, "Unknown KDC host `%s' (gai rc %d)", realm, rc);
      rc = SHISHI_KDC_NOT_KNOWN_FOR_REALM;
      goto done;
    }

  if (VERBOSE (handle))
    printf ("Sending to %s:%s (%s)...\n", realm, port,
	    inet_ntoa (((struct sockaddr_in *)ai->ai_addr)->sin_addr));

  rc = shishi_sendrecv_udp (handle, ai->ai_addr,
			    indata, inlen, outdata, outlen,
			    handle->kdctimeout);

 done:
  freeaddrinfo (ai);
  return rc;
}

int
shishi_kdc_sendrecv (Shishi * handle, char *realm,
		     const char *indata, size_t inlen,
		     char *outdata, size_t * outlen)
{
  int rc;

  rc = shishi_kdc_sendrecv_static (handle, realm,
				   indata, inlen, outdata, outlen);

  if (rc == SHISHI_KDC_TIMEOUT || rc == SHISHI_KDC_NOT_KNOWN_FOR_REALM)
    rc = shishi_kdc_sendrecv_srv (handle, realm,
				  indata, inlen, outdata, outlen);
  if (rc == SHISHI_KDC_TIMEOUT || rc == SHISHI_KDC_NOT_KNOWN_FOR_REALM)
    rc = shishi_kdc_sendrecv_direct (handle, realm,
				     indata, inlen, outdata, outlen);

  return rc;
}
