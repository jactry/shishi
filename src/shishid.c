/* shishid.c --- Shishi Key Distribution Center daemon.
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

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

/* Get setuid, read, etc. */
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

/* Get gethostbyname, getservbyname. */
#ifdef HAVE_NETDB_H
# include <netdb.h>
#endif

/* Get getpwnam. */
#ifdef HAVE_PWD_H
# include <pwd.h>
#endif

/* For select, etc. */
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

/* For select, etc. */
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

/* Get select, etc. */
#ifdef HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif

/* Get accept, sendto, etc. */
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif

/* Used for the backlog argument to listen. */
#ifndef SOMAXCONN
# define SOMAXCONN INT_MAX
#endif

#ifdef HAVE_SYS_IOCTL_H
# include <sys/ioctl.h>
#endif

/* Get errno. */
#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif
#ifndef errno
extern int errno;
#endif

#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

/* Get signal, etc. */
#ifdef HAVE_SIGNAL_H
# include <signal.h>
#endif

#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#ifdef HAVE_NETINET_IN6_H
# include <netinet/in6.h>
#endif

#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif

#ifdef HAVE_SYSLOG_H
# include <syslog.h>
#endif

#ifdef USE_STARTTLS
# include <gnutls/gnutls.h>
#endif

/* Setup i18n. */
#ifdef HAVE_LOCALE_H
# include <locale.h>
#else
# define setlocale(Category, Locale) /* empty */
#endif
#include <gettext.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)

/* Get xmalloc. */
#include "xalloc.h"

/* Get error. */
#include "error.h"

/* Get asprintf. */
#include "vasprintf.h"

/* Get program_name, etc. */
#include "progname.h"

/* Shishi and Shisa library. */
#include <shishi.h>
#include <shisa.h>

/* Command line parameter parser via gengetopt. */
#include "shishid_cmd.h"

#define FAMILY_IPV4 "IPv4"
#define FAMILY_IPV6 "IPv6"

#ifdef WITH_IPV6
# define LISTEN_DEFAULT FAMILY_IPV4 ":*:kerberos/udp, " \
			FAMILY_IPV4 ":*:kerberos/tcp, " \
			FAMILY_IPV6 ":*:kerberos/udp, " \
			FAMILY_IPV6 ":*:kerberos/tcp"
#else
# define LISTEN_DEFAULT "*:kerberos/udp, *:kerberos/tcp"
#endif

#define DH_BITS 1024
#ifdef USE_STARTTLS
static gnutls_dh_params dh_params;
static gnutls_anon_server_credentials anoncred;
#endif

static char *fatal_krberror;
static size_t fatal_krberror_len;

struct listenspec
{
  char *str;
  int family;
  int listening;
  struct sockaddr addr;
  socklen_t addrlen;
  struct sockaddr_in *sin;
  int port;
  int type;
  int sockfd;
  char buf[BUFSIZ]; /* XXX */
  size_t bufpos;
#ifdef USE_STARTTLS
  gnutls_session session;
#endif
  struct listenspec *next;
};

Shishi * handle;
Shisa * dbh;
struct gengetopt_args_info arg;
struct listenspec *listenspec;

static int
asreq1 (Shishi_as * as)
{
  Shishi_tkt *tkt;
  Shishi_key *serverkey = NULL, *sessionkey = NULL, *userkey = NULL;
  Shisa_key *serverdbkey = NULL, *userdbkey = NULL;
  Shisa_key **serverkeys, **userkeys;
  size_t nserverkeys, nuserkeys;
  int err;
  char *username, *servername, *realm;
  Shisa_principal krbtgt;
  Shisa_principal user;
  uint32_t etype;
  int i;

  /* Find the server, e.g., krbtgt/JOSEFSSON.ORG@JOSEFSSON.ORG. */

  err = shishi_kdcreq_server (handle, shishi_as_req (as), &servername, NULL);
  if (err != SHISHI_OK)
    return err;
  printf ("servername %s\n", servername);

  err = shishi_kdcreq_realm (handle, shishi_as_req (as), &realm, NULL);
  if (err != SHISHI_OK)
    return err;
  printf ("client & server realm %s\n", realm);

  err = shisa_principal_find (dbh, realm, servername, &krbtgt);
  if (err != SHISA_OK)
    {
      printf ("server %s@%s not found\n", servername, realm);
      return SHISHI_INVALID_PRINCIPAL_NAME;
    }
  printf ("Found server %s@%s...\n", servername, realm);

  /* Find the user, e.g., simon@JOSEFSSON.ORG. */

  err = shishi_kdcreq_client (handle, shishi_as_req (as), &username, NULL);
  if (err != SHISHI_OK)
    return err;
  printf ("username %s\n", username);

  err = shisa_principal_find (dbh, realm, username, &user);
  if (err != SHISA_OK)
    {
      printf ("user %s@%s not found\n", username, realm);
      return SHISHI_INVALID_PRINCIPAL_NAME;
    }
  printf ("Found user %s@%s...\n", username, realm);

  /* Enumerate keys for user and server. */

  err = shisa_enumerate_keys (dbh, realm, servername,
			      &serverkeys, &nserverkeys);
  if (err != SHISA_OK)
    {
      printf ("Error getting keys for %s@%s\n", servername, realm);
      return SHISHI_INVALID_PRINCIPAL_NAME;
    }
  printf ("Found keys for server %s@%s...\n", servername, realm);

  err = shisa_enumerate_keys (dbh, realm, username,
			      &userkeys, &nuserkeys);
  if (err != SHISA_OK)
    {
      printf ("Error getting keys for %s@%s\n", username, realm);
      return SHISHI_INVALID_PRINCIPAL_NAME;
    }
  printf ("Found keys for user %s@%s...\n", username, realm);

  /* Select keys in database that match supplied encryption type. */

  for (i = 1; (err = shishi_kdcreq_etype (handle, shishi_as_req (as),
					  &etype, i)) == SHISHI_OK; i++)
    {
      size_t j;
      printf ("Trying etype %d...\n", etype);
      if (!shishi_cipher_supported_p (etype))
	continue;
      if (serverdbkey == NULL)
	for (j = 0; j < nserverkeys; j++)
	  {
	    printf ("Matching against server etype %d...\n",
		    serverkeys[j]->etype);
	    if (serverkeys[j]->etype == etype)
		serverdbkey = serverkeys[j];
	  }
      if (userdbkey == NULL)
	for (j = 0; j < nuserkeys; j++)
	  {
	    printf ("Matching against user etype %d...\n",
		    userkeys[j]->etype);
	    if (userkeys[j]->etype == etype)
	      userdbkey = userkeys[j];
	  }
    }

  if (userdbkey == NULL)
    {
      printf ("No key found for %s@%s\n", username, realm);
      return SHISHI_INVALID_PRINCIPAL_NAME;
    }

  if (serverdbkey == NULL)
    {
      printf ("No key found for %s@%s\n", servername, realm);
      return SHISHI_INVALID_PRINCIPAL_NAME;
    }

  err = shishi_key_from_value (handle, userdbkey->etype,
			       userdbkey->key, &userkey);
  if (err != SHISHI_OK)
    return err;

  err = shishi_key_from_value (handle, serverdbkey->etype,
			       serverdbkey->key, &serverkey);
  if (err != SHISHI_OK)
    return err;

  /* Generate session key of same key type as the selected long-term
     server key. */

  err = shishi_key_random (handle, shishi_key_type (serverkey), &sessionkey);
  if (err)
    return err;

  /* Build Ticket and AS-REP. */

  tkt = shishi_as_tkt (as);

  err = shishi_tkt_key_set (tkt, sessionkey);
  if (err)
    return err;

  err = shishi_tkt_clientrealm_set (tkt, realm, username);
  if (err)
    return err;

  err = shishi_tkt_serverrealm_set (tkt, realm, servername);
  if (err)
    return err;

  err = shishi_tkt_build (tkt, serverkey);
  if (err)
    return err;

  err = shishi_as_rep_build (as, userkey);
  if (err)
    return err;

  if (arg.verbose_flag)
    {
      shishi_kdcreq_print (handle, stderr, shishi_as_req (as));
      shishi_encticketpart_print (handle, stderr,
				  shishi_tkt_encticketpart (tkt));
      shishi_ticket_print (handle, stderr, shishi_tkt_ticket (tkt));
      shishi_enckdcreppart_print (handle, stderr,
				  shishi_tkt_enckdcreppart (tkt));
      shishi_kdcrep_print (handle, stderr, shishi_as_rep (as));
    }

  return SHISHI_OK;
}

static int
tgsreq1 (Shishi_tgs * tgs)
{
  int rc;
  Shishi_tkt *tkt;
  Shishi_key *newsessionkey, *oldsessionkey, *serverkey, *subkey, *tgkey;
  char *servername, *serverrealm, *tgname, *tgrealm, *client, *clientrealm;
  Shisa_principal krbtgt;
  Shishi_asn1 reqapticket;
  Shisa_key **tgkeys;
  size_t ntgkeys;
  Shisa_key **serverkeys;
  size_t nserverkeys;
  int i;

  /* Extract pa-data and populate tgs->ap. */
  rc = shishi_tgs_req_process (tgs);
  if (rc != SHISHI_OK)
    return rc;

  /* Get ticket used to authenticate request. */
  rc = shishi_apreq_get_ticket (handle, shishi_ap_req (shishi_tgs_ap (tgs)),
				&reqapticket);
  if (rc != SHISHI_OK)
    return rc;

  /* Find name of ticket granter, e.g., krbtgt/JOSEFSSON.ORG@JOSEFSSON.ORG. */

  rc = shishi_ticket_realm_get (handle, reqapticket, &tgrealm, NULL);
  if (rc != SHISHI_OK)
    return rc;
  printf ("tg realm %s\n", tgrealm);

  rc = shishi_ticket_server (handle, reqapticket, &tgname, NULL);
  if (rc != SHISHI_OK)
    return rc;
  printf ("Found ticket granter name %s@%s...\n", tgname, tgrealm);

  /* We need to decrypt the ticket granting ticket, get key. */

  rc = shisa_enumerate_keys (dbh, tgrealm, tgname, &tgkeys, &ntgkeys);
  if (rc != SHISA_OK)
    {
      printf ("Error getting keys for %s@%s\n", tgname, tgrealm);
      return SHISHI_INVALID_PRINCIPAL_NAME;
    }
  printf ("Found keys for ticket granter %s@%s...\n", tgname, tgrealm);

  /* XXX use etype/kvno to select key. */

  rc = shishi_key_from_value (handle, tgkeys[0]->etype,
			       tgkeys[0]->key, &tgkey);
  if (rc != SHISHI_OK)
    return rc;

  shishi_key_print (handle, stdout, tgkey);

  /* Find the server, e.g., host/latte.josefsson.org@JOSEFSSON.ORG. */

  rc = shishi_kdcreq_realm (handle, shishi_tgs_req (tgs), &serverrealm, NULL);
  if (rc != SHISHI_OK)
    return rc;
  printf ("server realm %s\n", serverrealm);

  rc = shishi_kdcreq_server (handle, shishi_tgs_req (tgs), &servername, NULL);
  if (rc != SHISHI_OK)
    return rc;
  printf ("servername %s\n", servername);

  rc = shisa_principal_find (dbh, serverrealm, servername, &krbtgt);
  if (rc != SHISA_OK)
    {
      printf ("server %s@%s not found\n", servername, serverrealm);
      return SHISHI_INVALID_PRINCIPAL_NAME;
    }
  printf ("Found server %s@%s...\n", servername, serverrealm);

  /* Get key for server, used to encrypt new ticket. */

  rc = shisa_enumerate_keys (dbh, serverrealm, servername,
			     &serverkeys, &nserverkeys);
  if (rc != SHISA_OK)
    {
      printf ("Error getting keys for %s@%s\n", servername, serverrealm);
      return SHISHI_INVALID_PRINCIPAL_NAME;
    }
  printf ("Found keys for server %s@%s...\n", servername, serverrealm);

  /* XXX select "best" available key (tgs-req etype list, highest
     kvno, best algorithm?) here. */

  rc = shishi_key_from_value (handle, serverkeys[0]->etype,
			       serverkeys[0]->key, &serverkey);
  if (rc != SHISHI_OK)
    return rc;

  shishi_key_print (handle, stdout, serverkey);

  /* Decrypt incoming ticket with our key, and decrypt authenticator
     using key stored in ticket. */
  rc = shishi_ap_req_process_keyusage
    (shishi_tgs_ap (tgs), tgkey, SHISHI_KEYUSAGE_TGSREQ_APREQ_AUTHENTICATOR);
  if (rc != SHISHI_OK)
    return rc;

  /* XXX check that checksum in authenticator match tgsreq.req-body */

  tkt = shishi_tgs_tkt (tgs);

  /* Generate session key for the newly generated ticket, of same key
     type as the selected long-term server key. */

  rc = shishi_key_random (handle, shishi_key_type (serverkey),
			   &newsessionkey);
  if (rc)
    return rc;

  rc = shishi_tkt_key_set (tkt, newsessionkey);
  if (rc)
    return rc;

  /* In the new ticket, store identity of the client, taken from the
     decrypted incoming ticket. */

  rc = shishi_encticketpart_crealm
    (handle, shishi_tkt_encticketpart (shishi_ap_tkt (shishi_tgs_ap (tgs))),
     &clientrealm, NULL);
  if (rc != SHISHI_OK)
    return rc;
  printf ("userrealm %s\n", clientrealm);

  rc = shishi_encticketpart_client
    (handle, shishi_tkt_encticketpart (shishi_ap_tkt (shishi_tgs_ap (tgs))),
     &client, NULL);
  if (rc != SHISHI_OK)
    return rc;
  printf ("username %s\n", client);

  rc = shishi_tkt_clientrealm_set (tkt, clientrealm, client);
  if (rc)
    return rc;

  rc = shishi_tkt_serverrealm_set (tkt, serverrealm, servername);
  if (rc)
    return rc;

  /* Build new key, using the server's key. */

  rc = shishi_tkt_build (tkt, serverkey);
  if (rc)
    return rc;

  /* The TGS-REP need to be encrypted, decide which key to use.
     Either it is the session key in the incoming ticket, or it is the
     sub-key in the authenticator. */

  rc = shishi_encticketpart_get_key
    (handle,
     shishi_tkt_encticketpart (shishi_ap_tkt (shishi_tgs_ap (tgs))),
     &oldsessionkey);
  if (rc != SHISHI_OK)
    return rc;

  rc = shishi_authenticator_get_subkey
    (handle, shishi_ap_authenticator (shishi_tgs_ap (tgs)), &subkey);
  if (rc != SHISHI_OK && rc != SHISHI_ASN1_NO_ELEMENT)
    return rc;

  /* Build TGS-REP. */

  if (rc == SHISHI_OK)
    rc = shishi_tgs_rep_build
      (tgs, SHISHI_KEYUSAGE_ENCTGSREPPART_AUTHENTICATOR_KEY, subkey);
  else
    rc = shishi_tgs_rep_build
      (tgs, SHISHI_KEYUSAGE_ENCTGSREPPART_SESSION_KEY, oldsessionkey);
  if (rc)
    return rc;

  if (arg.verbose_flag)
    {
      puts ("KDC-REQ in:");
      shishi_kdcreq_print (handle, stderr, shishi_tgs_req (tgs));
      puts ("AP-REQ in KDC-REQ:");
      shishi_apreq_print (handle, stderr,
			  shishi_ap_req (shishi_tgs_ap (tgs)));
      puts ("Authenticator in AP-REQ in KDC-REQ:");
      shishi_authenticator_print (handle, stderr, shishi_ap_authenticator
				  (shishi_tgs_ap (tgs)));
      puts ("Ticket in AP-REQ:");
      shishi_ticket_print (handle, stdout,
			   shishi_tkt_ticket
			   (shishi_ap_tkt (shishi_tgs_ap (tgs))));
      puts ("EncTicketPart in AP-REQ:");
      shishi_encticketpart_print (handle, stdout,
				  shishi_tkt_encticketpart
				  (shishi_ap_tkt (shishi_tgs_ap (tgs))));
      puts ("Ticket in TGS-REP:");
      shishi_ticket_print (handle, stdout, shishi_tkt_ticket (tkt));
      puts ("EncTicketPart in TGS-REP:");
      shishi_encticketpart_print (handle, stderr,
				  shishi_tkt_encticketpart (tkt));
      puts ("EncKDCRepPart in TGS-REP:");
      shishi_enckdcreppart_print (handle, stderr,
				  shishi_tkt_enckdcreppart (tkt));
      puts ("KDC-REP:");
      shishi_kdcrep_print (handle, stderr, shishi_tgs_rep (tgs));
    }

  return SHISHI_OK;
}

static int
asreq (Shishi_asn1 kdcreq, char **out, size_t * outlen)
{
  Shishi_as *as;
  int rc;

  rc = shishi_as (handle, &as);
  if (rc != SHISHI_OK)
    {
      syslog (LOG_ERR, "Cannot create AS: %s\n", shishi_strerror (rc));
      return rc;
    }

  shishi_as_req_set (as, kdcreq);

  rc = asreq1 (as);
  if (rc != SHISHI_OK)
    {
      syslog (LOG_NOTICE, "Could not answer request: %s: %s\n",
	      shishi_strerror (rc),
	      shishi_krberror_message (handle, shishi_as_krberror (as)));
      rc = shishi_as_krberror_der (as, out, outlen);
    }
  else
    rc = shishi_as_rep_der (as, out, outlen);
  if (rc != SHISHI_OK)
    {
      syslog (LOG_ERR, "Cannot DER encode reply: %s\n", shishi_strerror (rc));
      return rc;
    }

  return SHISHI_OK;
}

static int
tgsreq (Shishi_asn1 kdcreq, char **out, size_t * outlen)
{
  Shishi_tgs *tgs;
  int rc;

  rc = shishi_tgs (handle, &tgs);
  if (rc != SHISHI_OK)
    {
      syslog (LOG_ERR, "Cannot create TGS: %s\n", shishi_strerror (rc));
      return rc;
    }

  shishi_tgs_req_set (tgs, kdcreq);

  rc = tgsreq1 (tgs);
  if (rc != SHISHI_OK)
    {
      syslog (LOG_NOTICE, "Could not answer request: %s: %s\n",
	      shishi_strerror (rc),
	      shishi_krberror_message (handle, shishi_tgs_krberror (tgs)));
      rc = shishi_tgs_krberror_der (tgs, out, outlen);
    }
  else
    rc = shishi_tgs_rep_der (tgs, out, outlen);
  if (rc != SHISHI_OK)
    {
      syslog (LOG_ERR, "Cannot DER encode reply: %s\n", shishi_strerror (rc));
      return rc;
    }

  return SHISHI_OK;
}

static int
process_1 (char *in, size_t inlen, char **out, size_t * outlen)
{
  Shishi_asn1 node;
  Shishi_asn1 krberr;
  int rc;

  krberr = shishi_krberror (handle);
  if (!krberr)
    return SHISHI_MALLOC_ERROR;

  node = shishi_der2asn1 (handle, in, inlen);

  fprintf (stderr, "ASN.1 msg-type %d (0x%x)...\n",
	   shishi_asn1_msgtype (handle, node),
	   shishi_asn1_msgtype (handle, node));

  switch (shishi_asn1_msgtype (handle, node))
    {
    case SHISHI_MSGTYPE_AS_REQ:
      puts ("Processing AS-REQ...");
      rc = asreq (node, out, outlen);
      break;

    case SHISHI_MSGTYPE_TGS_REQ:
      puts ("Processing TGS-REQ...");
      rc = tgsreq (node, out, outlen);
      break;

    default:
      rc = !SHISHI_OK;
      break;
    }

  if (rc != SHISHI_OK)
    {
      rc = shishi_krberror_set_etext (handle, krberr, "General error");
      if (rc != SHISHI_OK)
	return rc;

      rc = shishi_krberror_set_realm (handle, krberr, "unknown");
      if (rc != SHISHI_OK)
	return rc;

      rc = shishi_krberror_der (handle, krberr, out, outlen);
      if (rc != SHISHI_OK)
	return rc;
    }

  return SHISHI_OK;
}

static void
process (char *in, int inlen, char **out, size_t * outlen)
{
  int rc;

  *out = NULL;
  *outlen = 0;

  rc = process_1 (in, inlen, out, outlen);

  if (rc != SHISHI_OK || *out == NULL || *outlen == 0)
    {
      *out = fatal_krberror;
      *outlen = fatal_krberror_len;
    }
}

static void
kdc_listen ()
{
  struct listenspec *ls;
  int maxfd = 0;
  int i;
  int yes;

  for (ls = listenspec; ls; ls = ls->next)
    {
      if (!arg.quiet_flag)
	printf ("Listening on %s...", ls->str);

      ls->sockfd = socket (ls->family, ls->type, 0);
      if (ls->sockfd < 0)
	{
	  if (!arg.quiet_flag)
	    printf ("failed\n");
	  error (0, errno, "Cannot listen on %s because socket failed",
		 ls->str);
	  ls->sockfd = 0;
	  continue;
	}

      yes = 1;
      if (setsockopt (ls->sockfd, SOL_SOCKET, SO_REUSEADDR,
		      (char *) &yes, sizeof (yes)) < 0)
	{
	  if (!arg.quiet_flag)
	    printf ("failed\n");
	  error (0, errno, "Cannot listen on %s because setsockopt failed",
		 ls->str);
	  close (ls->sockfd);
	  ls->sockfd = 0;
	  continue;
	}

      if (bind (ls->sockfd, &ls->addr, sizeof (ls->addr)) != 0)
	{
	  if (!arg.quiet_flag)
	    printf ("failed\n");
	  error (0, errno, "Cannot listen on %s because bind failed",
		 ls->str);
	  close (ls->sockfd);
	  ls->sockfd = 0;
	  continue;
	}

      if (ls->type == SOCK_STREAM && listen (ls->sockfd, SOMAXCONN) != 0)
	{
	  if (!arg.quiet_flag)
	    printf ("failed\n");
	  error (0, errno, "Cannot listen on %s because listen failed",
		 ls->str);
	  close (ls->sockfd);
	  ls->sockfd = 0;
	  continue;
	}

      maxfd++;
      if (!arg.quiet_flag)
	printf ("done\n");
    }

  if (maxfd == 0)
    error (EXIT_FAILURE, 0, "Failed to bind any ports.");

  if (!arg.quiet_flag)
    printf ("Listening on %d ports...\n", maxfd);
}

static struct listenspec *
kdc_close (struct listenspec *ls)
{
  struct listenspec *tmp;
  int rc;

  if (ls->sockfd)
    {
      if (!arg.quiet_flag)
	printf ("Closing %s...", ls->str);
      rc = close (ls->sockfd);
      if (rc != 0)
	{
	  if (!arg.quiet_flag)
	    printf ("failed\n");
	  perror ("close");
	}
      else if (!arg.quiet_flag)
	printf ("done\n");
    }

  if (ls->str)
    free (ls->str);

  tmp = ls->next;

  free (ls);

  return tmp;
}

static void
kdc_unlisten (void)
{
  struct listenspec *ls = listenspec;

  while (ls)
    ls = kdc_close (ls);

  listenspec = NULL;
}

static void
kdc_handle2 (struct listenspec *ls)
{
  ssize_t sent_bytes, read_bytes;
  struct sockaddr addr;
  int rc;

#ifdef USE_STARTTLS
  if (ls->type == SOCK_STREAM &&
      ls->bufpos == 4 &&
      memcmp (ls->buf, "\x70\x00\x00\x01", 4) == 0)
    {
      const int kx_prio[] = { GNUTLS_KX_ANON_DH, 0 };

      if (!arg.quiet_flag)
	printf ("Trying to upgrade to TLS...\n");

      sent_bytes = sendto (ls->sockfd, "\x70\x00\x00\x02", 4,
			   0, &ls->addr, ls->addrlen);

      rc = gnutls_init (&ls->session, GNUTLS_SERVER);
      if (rc != GNUTLS_E_SUCCESS)
	error (EXIT_FAILURE, 0, "gnutls_init %d", rc);
      rc = gnutls_set_default_priority (ls->session);
      if (rc != GNUTLS_E_SUCCESS)
	error (EXIT_FAILURE, 0, "gnutls_sdp %d", rc);
      rc = gnutls_kx_set_priority (ls->session, kx_prio);
      if (rc != GNUTLS_E_SUCCESS)
	error (EXIT_FAILURE, 0, "gnutls_ksp %d", rc);
      rc = gnutls_credentials_set (ls->session, GNUTLS_CRD_ANON,
				   anoncred);
      if (rc != GNUTLS_E_SUCCESS)
	error (EXIT_FAILURE, 0, "gnutls_cs %d", rc);
      gnutls_dh_set_prime_bits (ls->session, DH_BITS);
      gnutls_transport_set_ptr (ls->session,
				(gnutls_transport_ptr)
				ls->sockfd);

      rc = gnutls_handshake (ls->session);
      if (rc < 0)
	printf ("Handshake has failed %d: %s\n",
		rc, gnutls_strerror (rc));
      else
	{
	  if (!arg.quiet_flag)
	    printf ("TLS successful\n");

	  rc = gnutls_record_recv (ls->session, ls->buf,
				   sizeof (ls->buf) - 1);

	  if (rc == 0)
	    {
	      printf ("Peer has closed the GNUTLS connection\n");
	    }
	  else if (rc < 0)
	    {
	      printf ("Corrupted data(%d).\n", rc);
	    }
	  else if (rc > 0)
	    {
	      char *p;
	      size_t plen;

	      process (ls->buf, rc, &p, &plen);

	      printf ("TLS process %d sending %d\n", rc, plen);

	      gnutls_record_send (ls->session, p, plen);

	      if (p != fatal_krberror)
		free (p);
	    }
	  ls->bufpos = 0;
	}
      gnutls_bye (ls->session, GNUTLS_SHUT_WR);
      gnutls_deinit (ls->session);
    }
  else
#endif
    if (ls->type == SOCK_DGRAM ||
	(ls->bufpos > 4 &&
	 ntohl (*(int *) ls->buf) + 4 == ls->bufpos))
      {
	char *p;
	size_t plen;

	if (ls->type == SOCK_STREAM)
	  process (ls->buf + 4, ls->bufpos - 4, &p, &plen);
	else
	  process (ls->buf, ls->bufpos, &p, &plen);

	if (p && plen > 0)
	  {
	    if (ls->type == SOCK_STREAM)
	      {
		uint32_t len = htonl (plen) + 4;
		do
		  sent_bytes = sendto (ls->sockfd, &len, 4,
				       0, &ls->addr, ls->addrlen);
		while (sent_bytes == -1 && errno == EAGAIN);
	      }

	    do
	      sent_bytes = sendto (ls->sockfd, p, plen,
				   0, &ls->addr, ls->addrlen);
	    while (sent_bytes == -1 && errno == EAGAIN);

	    if (sent_bytes < 0)
	      perror ("write");
	    else if ((size_t) sent_bytes > plen)
	      fprintf (stderr, "wrote %db but buffer only %db",
		       sent_bytes, plen);
	    else if ((size_t) sent_bytes < plen)
	      fprintf (stderr,
		       "short write (%db) writing %d bytes\n",
		       sent_bytes, plen);

	    if (p != fatal_krberror)
	      free (p);
	  }

	ls->bufpos = 0;
      }
}

static int
kdc_read (struct listenspec *ls)
{
  ssize_t read_bytes;

  read_bytes = recvfrom (ls->sockfd, ls->buf + ls->bufpos,
			 sizeof(ls->buf) - ls->bufpos, 0,
			 &ls->addr, &ls->addrlen);

  if (read_bytes < 0)
    {
      error (0, errno, "Error from recvfrom (%d)", read_bytes);
      return -1;
    }

  if (read_bytes == 0 && ls->type == SOCK_STREAM)
    {
      if (!arg.quiet_flag)
	printf ("Peer %s disconnected\n", ls->str);
      return -1;
    }

  ls->bufpos += read_bytes;
  ls->buf[ls->bufpos] = '\0';

  if (!arg.quiet_flag)
    printf ("Has %d bytes from %s on socket %d\n",
	    ls->bufpos, ls->str, ls->sockfd);

  return 0;
}

static void
kdc_accept (struct listenspec *ls, struct listenspec *last)
{
  struct listenspec *newls;

  newls = xzalloc (sizeof (*newls));
  newls->next = ls->next;
  ls->next = newls;

  newls->bufpos = 0;
  newls->type = ls->type;
  newls->addrlen = sizeof (newls->addr);
  newls->sockfd = accept (ls->sockfd, &newls->addr, &newls->addrlen);
  newls->sin = (struct sockaddr_in *) &newls->addr;
  asprintf (&newls->str, "%s peer %s", ls->str,
	    inet_ntoa (newls->sin->sin_addr));

  if (!arg.quiet_flag)
    printf ("Accepted socket %d from socket %d as %s\n",
	    newls->sockfd, ls->sockfd, newls->str);
}

int quit = 0;

static void
ctrlc (int signum)
{
  quit = 1;
}

#define MAX(a,b) ((a) > (b) ? (a) : (b))

static void
kdc_loop (void)
{
  struct listenspec *ls, *last;
  fd_set readfds;
  int maxfd = 0, i;
  int rc;

  signal (SIGINT, ctrlc);
  signal (SIGTERM, ctrlc);

  while (!quit)
    {
      do
	{
	  FD_ZERO (&readfds);
	  maxfd = 0;
	  for (ls = listenspec; ls; ls = ls->next)
	    {
	      maxfd = MAX(maxfd, ls->sockfd + 1);
	      if (!arg.quiet_flag)
		printf ("Listening on socket %d\n", ls->sockfd);
	      FD_SET (ls->sockfd, &readfds);
	    }
	}
      while ((rc = select (maxfd, &readfds, NULL, NULL, NULL)) == 0);

      if (rc < 0)
	{
	  if (errno != EINTR)
	    error (0, errno, "Error listening to sockets (%d)", rc);
	  continue;
	}

      for (ls = listenspec, last = NULL; ls; last = ls, ls = ls->next)
	if (FD_ISSET (ls->sockfd, &readfds))
	  if (ls->type == SOCK_STREAM && ls->listening)
	    kdc_accept (ls, last);
	  else if (kdc_read (ls) < 0)
	    last->next = kdc_close (ls);
	  else
	    kdc_handle2 (ls);
    }
}

static void
kdc_setuid (void)
{
  struct passwd *passwd;
  int rc;

  if (!arg.setuid_given)
    return;

  passwd = getpwnam (arg.setuid_arg);
  if (passwd == NULL)
    if (errno)
      error (EXIT_FAILURE, errno, "Cannot setuid because getpwnam failed");
    else
      error (EXIT_FAILURE, 0, "No such user `%s'.", arg.setuid_arg);

  rc = setuid (passwd->pw_uid);
  if (rc == -1)
    error (EXIT_FAILURE, errno, "Cannot setuid");

  if (!arg.quiet_flag)
    printf ("User identity set to `%s' (%d)...\n",
	    passwd->pw_name, passwd->pw_uid);
}

static int
setup_fatal_krberror (Shishi * handle)
{
  Shishi_asn1 krberr;
  int rc;

  krberr = shishi_krberror (handle);
  if (!krberr)
    return SHISHI_MALLOC_ERROR;

  rc = shishi_krberror_set_etext (handle, krberr,
				  "Internal KDC error, contact administrator");
  if (rc != SHISHI_OK)
    return rc;

  rc = shishi_krberror_der (handle, krberr, &fatal_krberror,
			    &fatal_krberror_len);
  if (rc != SHISHI_OK)
    return rc;

  return SHISHI_OK;
}

static void
doit (void)
{
  int err;

  err = shishi_init_server_with_paths (&handle, arg.configuration_file_arg);
  if (err)
    error (EXIT_FAILURE, 0, "Cannot initialize Shishi: %s (%d)",
	   shishi_strerror (err), err);

  if (arg.verbose_flag > 1)
    shishi_cfg (handle, "verbose");

  if (arg.verbose_flag > 2)
    shishi_cfg (handle, "verbose-noice");

  if (arg.verbose_flag > 3)
    shishi_cfg (handle, "verbose-asn1");

  if (arg.verbose_flag > 4)
    shishi_cfg (handle, "verbose-crypto");

  err = shisa_init (&dbh);
  if (err)
    error (EXIT_FAILURE, 0, "Cannot initialize Shisa: %s (%d)",
	   shisa_strerror (err), err);

  err = setup_fatal_krberror (handle);
  if (err)
    error (EXIT_FAILURE, 0, "Cannot allocate fatal error packet: %s (%d)",
	   shisa_strerror (err), err);

#ifdef USE_STARTTLS
  if (!arg.quiet_flag)
    printf ("Initializing GNUTLS...\n");
  err = gnutls_global_init ();
  if (err)
    error (EXIT_FAILURE, 0, "Cannot initialize GNUTLS: %s (%d)",
	   gnutls_strerror (err), err);
  err = gnutls_dh_params_init (&dh_params);
  if (err)
    error (EXIT_FAILURE, 0, "Cannot initialize GNUTLS DH parameters: %s (%d)",
	   gnutls_strerror (err), err);
  err = gnutls_dh_params_generate2 (dh_params, DH_BITS);
  if (err)
    error (EXIT_FAILURE, 0, "Cannot generate GNUTLS DH parameters: %s (%d)",
	   gnutls_strerror (err), err);
  err = gnutls_anon_allocate_server_credentials (&anoncred);
  if (err)
    error (EXIT_FAILURE, 0, "Cannot allocate GNUTLS credential: %s (%d)",
	   gnutls_strerror (err), err);
  gnutls_anon_set_server_dh_params (anoncred, dh_params);
  if (!arg.quiet_flag)
    printf ("Initializing GNUTLS...done\n");
#endif

  kdc_listen ();

#ifdef LOG_PERROR
  openlog (PACKAGE, LOG_CONS | LOG_PERROR, LOG_DAEMON);
#else
  openlog (PACKAGE, LOG_CONS, LOG_DAEMON);
#endif

  kdc_setuid ();

  kdc_loop ();

  kdc_unlisten ();

#ifdef USE_STARTTLS
  if (!arg.quiet_flag)
    printf ("Deinitializing GNUTLS...\n");
  gnutls_global_deinit ();
  if (!arg.quiet_flag)
    printf ("Deinitializing GNUTLS...done\n");
#endif

  shisa_done (dbh);
  shishi_done (handle);
}

static void
parse_listen (char *listen)
{
  char *ptrptr;
  char *val;
  int i;

  for (i = 0; (val = strtok_r (i == 0 ? listen : NULL, ", \t", &ptrptr)); i++)
    {
      char *service, *proto;
      struct servent *se;
      struct hostent *he;
      struct listenspec *ls;
      struct sockaddr_in *sin;
#ifdef WITH_IPV6
      struct sockaddr_in6 *sin6;
#endif

      ls = xzalloc (sizeof (*ls));
      ls->next = listenspec;
      listenspec = ls;

      ls->str = strdup (val);
      ls->bufpos = 0;
      ls->listening = 1;
      sin = (struct sockaddr_in *) &ls->addr;
#ifdef WITH_IPV6
      sin6 = (struct sockaddr_in6 *) &ls->addr;
#endif

      proto = strrchr (val, '/');
      if (proto == NULL)
	error (1, 0, "Could not find type in listen spec: `%s'", ls->str);
      *proto = '\0';
      proto++;

      if (strcmp (proto, "tcp") == 0)
	ls->type = SOCK_STREAM;
      else
	ls->type = SOCK_DGRAM;

      service = strrchr (val, ':');
      if (service == NULL)
	error (1, 0, "Could not find service in listen spec: `%s'", ls->str);
      *service = '\0';
      service++;

      se = getservbyname (service, proto);
      if (se)
	ls->port = ntohs (se->s_port);
      else if (strcmp (service, "kerberos") == 0)
	ls->port = 88;
      else if (atoi (service) != 0)
	ls->port = atoi (service);
      else
	error (1, 0, "Unknown service `%s' in listen spec: `%s'",
	       service, ls->str);

#ifdef WITH_IPV6
      if (ls->family == AF_INET6)
	sin6->sin6_port = htons (ls->port);
      else
#endif
	sin->sin_port = htons (ls->port);

      if (strncmp (val, FAMILY_IPV4 ":", strlen (FAMILY_IPV4 ":")) == 0)
	{
	  ls->family = AF_INET;
	  val += strlen (FAMILY_IPV4 ":");
	}
#ifdef WITH_IPV6
      else if (strncmp (val, FAMILY_IPV6 ":", strlen (FAMILY_IPV6 ":")) ==
	       0)
	{
	  ls->family = AF_INET6;
	  val += strlen (FAMILY_IPV6 ":");
	}
#endif
      else
	ls->family = AF_INET;

      if (strcmp (val, "*") == 0)
	{
#ifdef WITH_IPV6
	  if (ls->family == AF_INET6)
	    sin6->sin6_addr = in6addr_any;
	  else
#endif
	    sin->sin_addr.s_addr = htonl (INADDR_ANY);
	}
      else if ((he = gethostbyname (val)))
	{
	  if (he->h_addrtype == AF_INET)
	    {
	      sin->sin_family = AF_INET;
	      memcpy (&sin->sin_addr, he->h_addr_list[0], he->h_length);
	    }
#ifdef WITH_IPV6
	  else if (he->h_addrtype == AF_INET6)
	    {
	      sin6->sin6_family = AF_INET6;
	      memcpy (&sin6->sin6_addr, he->h_addr_list[0], he->h_length);
	    }
#endif
	  else
	    error (1, 0, "Unknown protocol family (%d) returned "
		   "by gethostbyname(\"%s\"): `%s'", he->h_addrtype,
		   val, ls->str);
	}
      else
	error (1, 0, "Unknown host `%s' in listen spec: `%s'", val, ls->str);
    }
}

int
main (int argc, char *argv[])
{
  int rc;

  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  set_program_name (argv[0]);

  if (cmdline_parser (argc, argv, &arg) != 0)
    error (EXIT_FAILURE, 0, "Try `%s --help' for more information.",
	   program_name);

  if (arg.help_given)
    {
      cmdline_parser_print_help ();
      printf ("\nMandatory arguments to long options are "
	      "mandatory for short options too.\n\n");
      printf ("Report bugs to <%s>.\n", PACKAGE_BUGREPORT);
      return EXIT_SUCCESS;
    }

  if (!arg.configuration_file_arg)
    arg.configuration_file_arg = strdup (SYSTEMCFGFILE);
  if (!arg.listen_given)
    arg.listen_arg = strdup (LISTEN_DEFAULT);
  parse_listen (arg.listen_arg);

  doit ();

  free (arg.listen_arg);
  free (arg.configuration_file_arg);
  if (arg.setuid_arg)
    free (arg.setuid_arg);

  return EXIT_SUCCESS;
}
