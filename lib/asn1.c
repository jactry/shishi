/* asn1.c --- Utilities to manipulate RFC 1510 ASN.1 types.
 * Copyright (C) 2002, 2003, 2004  Simon Josefsson
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

#include <libtasn1.h>
#define _SHISHI_HAS_LIBTASN1_H 1
#include "internal.h"
#include "asn1.h"

/* Defined in kerberos5.c, generated from kerberos5.asn1. */
extern const ASN1_ARRAY_TYPE shishi_asn1_tab[];

/* Prototype in asn1.h, used by init.c. */
int
_shishi_asn1_init (Shishi * handle)
{
  char errorDescription[MAX_ERROR_DESCRIPTION_SIZE] = "";
  int asn1_result;

  if (!asn1_check_version (LIBTASN1_VERSION))
    {
      shishi_warn (handle, "asn1_check-version(%s) failed: %s",
		   LIBTASN1_VERSION, asn1_check_version (NULL));
      return SHISHI_ASN1_ERROR;
    }

  if (!asn1_check_version ("0.2.5"))
    shishi_warn (handle, "libtasn1 >= 0.2.5 preferred, you may see bugs.");

  asn1_result = asn1_array2tree (shishi_asn1_tab,
				 &handle->asn1, errorDescription);
  if (asn1_result != ASN1_SUCCESS)
    {
      shishi_warn (handle, "asn1_array2tree() failed: %s\n",
		   libtasn1_strerror (asn1_result));
      shishi_warn (handle, "%s", errorDescription);
      return SHISHI_ASN1_ERROR;
    }

  return SHISHI_OK;
}

int
shishi_asn1_number_of_elements (Shishi * handle, Shishi_asn1 node,
				const char *field, size_t * n)
{
  int rc;
  int tmp;

  rc = asn1_number_of_elements (node, field, &tmp);
  *n = tmp;
  if (rc != ASN1_SUCCESS)
    {
      if (rc == ASN1_ELEMENT_NOT_FOUND)
	return SHISHI_ASN1_NO_ELEMENT;
      else
	return SHISHI_ASN1_ERROR;
    }

  return SHISHI_OK;
}

int
shishi_asn1_empty_p (Shishi * handle, Shishi_asn1 node, const char *field)
{
  int rc;
  int datalen;

  datalen = 0;
  rc = asn1_read_value (node, field, NULL, &datalen);
  if (rc == ASN1_VALUE_NOT_FOUND)
    return 1;

  return 0;
}

/* XXX obsolete, see shishi_asn1_read2 */
int
shishi_asn1_read (Shishi * handle, Shishi_asn1 node,
		  const char *field, char *data, size_t * datalen)
{
  int rc;

  rc = asn1_read_value (node, field, (unsigned char *) data, (int *) datalen);
  if (rc != ASN1_SUCCESS)
    {
      shishi_error_set (handle, libtasn1_strerror (rc));
      if (rc == ASN1_ELEMENT_NOT_FOUND)
	return SHISHI_ASN1_NO_ELEMENT;
      else if (rc == ASN1_VALUE_NOT_FOUND)
	return SHISHI_ASN1_NO_VALUE;
      else
	return SHISHI_ASN1_ERROR;
    }

  return SHISHI_OK;
}

/* XXX rename and document */
int
shishi_asn1_read2 (Shishi * handle,
		   Shishi_asn1 node, const char *field,
		   char **data, size_t * datalen)
{
  int rc;
  int len = 0;

  rc = asn1_read_value (node, field, NULL, &len);
  if (rc != ASN1_SUCCESS && rc != ASN1_MEM_ERROR)
    {
      shishi_error_set (handle, libtasn1_strerror (rc));
      if (rc == ASN1_ELEMENT_NOT_FOUND)
	return SHISHI_ASN1_NO_ELEMENT;
      else if (rc == ASN1_VALUE_NOT_FOUND)
	return SHISHI_ASN1_NO_VALUE;
      else
	return SHISHI_ASN1_ERROR;
    }

  if (data)
    {
      size_t dlen = (size_t) len;

      *data = xmalloc (len + 1);

      if (len > 0)
	{
	  rc = shishi_asn1_read (handle, node, field, *data, &dlen);
	  if (rc != SHISHI_OK)
	    return rc;
	}

      (*data)[len] = '\0';
    }

  if (datalen)
    *datalen = (size_t) len;

  return SHISHI_OK;
}

int
shishi_asn1_read_int32 (Shishi * handle, Shishi_asn1 node,
			const char *field, int32_t * i)
{
  unsigned char buf[4];
  size_t buflen;
  int rc;

  memset (buf, 0, sizeof (buf));
  buflen = sizeof (buf);
  rc = shishi_asn1_read (handle, node, field, buf, &buflen);
  if (rc != SHISHI_OK)
    return rc;

  if (buflen < 4)
    {
      memset (buf, 0, sizeof (buf));
      rc = shishi_asn1_read (handle, node, field, &buf[4 - buflen], &buflen);
      if (rc != SHISHI_OK)
	return rc;
    }
  *i = buf[3] | (buf[2] << 8) | (buf[1] << 16) | (buf[0] << 24);

  return SHISHI_OK;
}

int
shishi_asn1_read_uint32 (Shishi * handle, Shishi_asn1 node,
			 const char *field, uint32_t * i)
{
  return shishi_asn1_read_int32 (handle, node, field, (int32_t *) i);
}

int
shishi_asn1_read_integer (Shishi * handle, Shishi_asn1 node,
			  const char *field, int *i)
{
  return shishi_asn1_read_int32 (handle, node, field, (int32_t *) i);
}

int
shishi_asn1_read_bitstring (Shishi * handle, Shishi_asn1 node,
			    const char *field, uint32_t * flags)
{
  char *buf;
  size_t buflen;
  size_t i;
  int res;

  res = shishi_asn1_read2 (handle, node, field, &buf, &buflen);
  if (res != SHISHI_OK)
    return res;

  if (buflen < 4)
    return SHISHI_ASN1_ERROR;

  *flags = 0;
  for (i = 0; i < 4; i++)
    {
      *flags |= (((buf[i] >> 7) & 0x01) |
		 ((buf[i] >> 5) & 0x02) |
		 ((buf[i] >> 3) & 0x04) |
		 ((buf[i] >> 1) & 0x08) |
		 ((buf[i] << 1) & 0x10) |
		 ((buf[i] << 3) & 0x20) |
		 ((buf[i] << 5) & 0x40) | ((buf[i] << 7) & 0x80)) << (8 * i);
    }

  return SHISHI_OK;
}

/* XXX obsolete, see shishi_asn1_read2_optional */
int
shishi_asn1_read_optional (Shishi * handle,
			   Shishi_asn1 node, const char *field,
			   char *data, size_t * datalen)
{
  int rc;

  rc = asn1_read_value (node, field, (unsigned char *) data, (int *) datalen);
  if (rc != ASN1_SUCCESS && rc != ASN1_ELEMENT_NOT_FOUND)
    {
      shishi_error_set (handle, libtasn1_strerror (rc));
      return SHISHI_ASN1_ERROR;
    }

  if (rc == ASN1_ELEMENT_NOT_FOUND)
    *datalen = 0;

  return SHISHI_OK;
}

/* XXX rename and document */
int
shishi_asn1_read2_optional (Shishi * handle,
			    Shishi_asn1 node, const char *field,
			    char **data, size_t * datalen)
{
  int rc;

  rc = shishi_asn1_read2 (handle, node, field, data, datalen);
  if (rc != SHISHI_OK && rc != SHISHI_ASN1_NO_ELEMENT)
    return rc;

  if (rc == SHISHI_ASN1_NO_ELEMENT)
    if (datalen)
      *datalen = 0;

  return SHISHI_OK;
}

int
shishi_asn1_write (Shishi * handle, Shishi_asn1 node,
		   const char *field, const char *data, size_t datalen)
{
  int rc;

  rc = asn1_write_value (node, field,
			 (const unsigned char *) data, (int) datalen);
  if (rc != ASN1_SUCCESS)
    {
      shishi_error_set (handle, libtasn1_strerror (rc));
      return SHISHI_ASN1_ERROR;
    }

  return SHISHI_OK;
}

int
shishi_asn1_write_uint32 (Shishi * handle, Shishi_asn1 node,
			  const char *field, uint32_t n)
{
  char *buf;
  int res;

  asprintf (&buf, "%lu", n);
  res = shishi_asn1_write (handle, node, field, buf, 0);
  free (buf);
  if (res != SHISHI_OK)
    return res;

  return SHISHI_OK;
}

int
shishi_asn1_write_int32 (Shishi * handle, Shishi_asn1 node,
			 const char *field, int32_t n)
{
  char *buf;
  int res;

  asprintf (&buf, "%ld", n);
  res = shishi_asn1_write (handle, node, field, buf, 0);
  free (buf);
  if (res != SHISHI_OK)
    return res;

  return SHISHI_OK;
}

int
shishi_asn1_write_integer (Shishi * handle, Shishi_asn1 node,
			   const char *field, int n)
{
  return shishi_asn1_write_int32 (handle, node, field, (int32_t) n);
}

int
shishi_asn1_write_bitstring (Shishi * handle, Shishi_asn1 node,
			     const char *field, uint32_t flags)
{
  unsigned char buf[4];
  size_t i;
  int res;

  /* XXX
     Cannot handle bit strings longer than 32 bits.
     Currently not needed though. */

  for (i = 0; i < 4; i++)
    {
      buf[i] = ((((flags >> (8 * i)) >> 7) & 0x01) |
		(((flags >> (8 * i)) >> 5) & 0x02) |
		(((flags >> (8 * i)) >> 3) & 0x04) |
		(((flags >> (8 * i)) >> 1) & 0x08) |
		(((flags >> (8 * i)) << 1) & 0x10) |
		(((flags >> (8 * i)) << 3) & 0x20) |
		(((flags >> (8 * i)) << 5) & 0x40) |
		(((flags >> (8 * i)) << 7) & 0x80));
    }

  res = shishi_asn1_write (handle, node, field, buf, 32);
  if (res != SHISHI_OK)
    return res;

  return SHISHI_OK;
}

/**
 * shishi_asn1_done:
 * @handle: shishi handle as allocated by shishi_init().
 * @node: ASN.1 node to dellocate.
 *
 * Deallocate resources associated with ASN.1 structure.  Note that
 * the node must not be used after this call.
 **/
void
shishi_asn1_done (Shishi * handle, Shishi_asn1 node)
{

  int rc;

  rc = asn1_delete_structure (&node);
  if (rc != ASN1_SUCCESS)
    shishi_error_printf (handle, "Cannot dellocate ASN.1 structure: %s",
			 libtasn1_strerror (rc));
}

static Shishi_asn1
asn1_new (Shishi * handle, const char *field, const char *name)
{
  ASN1_TYPE node = ASN1_TYPE_EMPTY;
  int res;

  res = asn1_create_element (handle->asn1, field, &node);
  if (res != ASN1_SUCCESS)
    {
      shishi_error_set (handle, libtasn1_strerror (res));
      return NULL;
    }

  return (Shishi_asn1) node;
}

/**
 * shishi_asn1_asreq:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for AS-REQ.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_asreq (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.AS-REQ", "KDC-REQ");
}

/**
 * shishi_asn1_asrep:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for AS-REP.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_asrep (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.AS-REP", "KDC-REP");
}

/**
 * shishi_asn1_tgsreq:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for TGS-REQ.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_tgsreq (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.TGS-REQ", "KDC-REQ");
}

/**
 * shishi_asn1_tgsrep:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for TGS-REP.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_tgsrep (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.TGS-REP", "KDC-REP");
}

/**
 * shishi_asn1_apreq:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for AP-REQ.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_apreq (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.AP-REQ", "AP-REQ");
}

/**
 * shishi_asn1_aprep:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for AP-REP.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_aprep (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.AP-REP", "AP-REP");
}

/**
 * shishi_asn1_encapreppart:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for AP-REP.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_encapreppart (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.EncAPRepPart", "EncAPRepPart");
}

/**
 * shishi_asn1_ticket:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for Ticket.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_ticket (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.Ticket", "Ticket");
}

/**
 * shishi_asn1_encticketpart:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for EncTicketPart.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_encticketpart (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.EncTicketPart", "EncTicketPart");
}

/**
 * shishi_asn1_authenticator:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for Authenticator.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_authenticator (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.Authenticator", "Authenticator");
}

/**
 * shishi_asn1_enckdcreppart:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for EncKDCRepPart.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_enckdcreppart (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.EncKDCRepPart", "EncKDCRepPart");
}

/**
 * shishi_asn1_encasreppart:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for EncASRepPart.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_encasreppart (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.EncASRepPart", "EncKDCRepPart");
}

/**
 * shishi_asn1_krberror:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for KRB-ERROR.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_krberror (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.KRB-ERROR", "KRB-ERROR");
}

/**
 * shishi_asn1_krbsafe:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for KRB-SAFE.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_krbsafe (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.KRB-SAFE", "KRB-SAFE");
}

/**
 * shishi_asn1_priv:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for KRB-PRIV.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_priv (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.KRB-PRIV", "KRB-PRIV");
}

/**
 * shishi_asn1_encprivpart:
 * @handle: shishi handle as allocated by shishi_init().
 *
 * Create new ASN.1 structure for EncKrbPrivPart.
 *
 * Return value: Returns ASN.1 structure.
 **/
Shishi_asn1
shishi_asn1_encprivpart (Shishi * handle)
{
  return asn1_new (handle, "Kerberos5.EncKrbPrivPart", "EncKrbPrivPart");
}

/**
 * shishi_asn1_to_der_field:
 * @handle: shishi handle as allocated by shishi_init().
 * @node: ASN.1 data that have field to extract.
 * @field: name of field in @node to extract.
 * @der: output array that holds DER encoding of @field in @node.
 * @len: output variable with length of @der output array.
 *
 * Extract newly allocated DER representation of specified ASN.1 field.
 *
 * Return value: Returns SHISHI_OK if successful, or SHISHI_ASN1_ERROR
 *   if DER encoding fails (common reasons for this is that the ASN.1
 *   is missing required values).
 **/
int
shishi_asn1_to_der_field (Shishi * handle, Shishi_asn1 node,
			  const char *field, char **der, size_t * len)
{
  char errorDescription[MAX_ERROR_DESCRIPTION_SIZE] = "";
  int mylen = 0;
  int rc;

  rc = asn1_der_coding (node, field, NULL, &mylen, errorDescription);
  if (rc != ASN1_MEM_ERROR)
    {
      shishi_error_set (handle, errorDescription);
      return SHISHI_ASN1_ERROR;
    }

  *der = xmalloc (mylen);

  rc = asn1_der_coding (node, field, *der, &mylen, errorDescription);
  if (rc != ASN1_SUCCESS)
    {
      shishi_error_set (handle, errorDescription);
      return SHISHI_ASN1_ERROR;
    }

  *len = mylen;

  return SHISHI_OK;
}

/**
 * shishi_asn1_to_der:
 * @handle: shishi handle as allocated by shishi_init().
 * @node: ASN.1 data to convert to DER.
 * @der: output array that holds DER encoding of @node.
 * @len: output variable with length of @der output array.
 *
 * Extract newly allocated DER representation of specified ASN.1 data.
 *
 * Return value: Returns SHISHI_OK if successful, or SHISHI_ASN1_ERROR
 *   if DER encoding fails (common reasons for this is that the ASN.1
 *   is missing required values).
 **/
int
shishi_asn1_to_der (Shishi * handle, Shishi_asn1 node, char **der,
		    size_t * len)
{
  return shishi_asn1_to_der_field (handle, node, "", der, len);
}

static Shishi_asn1
der2asn1 (Shishi * handle,
	  const char *fieldname,
	  const char *nodename, const char *der, size_t derlen)
{
  char errorDescription[MAX_ERROR_DESCRIPTION_SIZE] = "";
  Shishi_asn1 structure = NULL;
  int asn1_result = ASN1_SUCCESS;

  asn1_result = asn1_create_element (handle->asn1, fieldname, &structure);
  if (asn1_result != ASN1_SUCCESS)
    {
      shishi_error_set (handle, libtasn1_strerror (asn1_result));
      return NULL;
    }

  asn1_result = asn1_der_decoding (&structure, (const unsigned char *) der,
				   (int) derlen, errorDescription);
  if (asn1_result != ASN1_SUCCESS)
    {
      asn1_delete_structure (&structure);
      shishi_error_set (handle, errorDescription);
      return NULL;
    }

  return structure;
}

/**
 * shishi_asn1_msgtype:
 * @handle: shishi handle as allocated by shishi_init().
 * @node: ASN.1 type to get msg type for.
 *
 * Determine msg-type of ASN.1 type of a Kerberos Protocol
 * packet. Currently this uses the msg-type field instead of the
 * APPLICATION tag, but this may be changed in the future.
 *
 * Return value: Returns msg-type of ASN.1 type, 0 on failure.
 **/
Shishi_msgtype
shishi_asn1_msgtype (Shishi * handle, Shishi_asn1 node)
{
  asn1_retCode rc;
  uint32_t msgtype;

  /* XXX Use APPLICATION tag instead. */
  rc = shishi_asn1_read_uint32 (handle, node, "msg-type", &msgtype);
  if (rc != SHISHI_OK)
    return 0;

  return msgtype;
}

/**
 * shishi_der_msgtype:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Determine msg-type of DER coded data of a Kerberos Protocol packet.
 *
 * Return value: Returns msg-type of DER data, 0 on failure.
 **/
Shishi_msgtype
shishi_der_msgtype (Shishi * handle, const char *der, size_t derlen)
{
  /* XXX Doesn't handle APPLICATION TAGS > 31. */
  if (derlen > 1 && *der >= 0x60 && (unsigned char) *der <= 0x7F)
    return *der - 0x60;
  else
    return 0;
}

/**
 * shishi_der2asn1:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Convert arbitrary DER data of a Kerberos Protocol packet to a
 * Kerberos ASN.1 type.
 *
 * Return value: Returns newly allocate ASN.1 corresponding to DER
 *   data, or %NULL on failure.
 **/
Shishi_asn1
shishi_der2asn1 (Shishi * handle, const char *der, size_t derlen)
{
  Shishi_asn1 node;

  switch (shishi_der_msgtype (handle, der, derlen))
    {
    case SHISHI_MSGTYPE_AS_REQ:
      node = shishi_der2asn1_asreq (handle, der, derlen);
      break;

    case SHISHI_MSGTYPE_AS_REP:
      node = shishi_der2asn1_asrep (handle, der, derlen);
      break;

    case SHISHI_MSGTYPE_TGS_REQ:
      node = shishi_der2asn1_tgsreq (handle, der, derlen);
      break;

    case SHISHI_MSGTYPE_TGS_REP:
      node = shishi_der2asn1_tgsrep (handle, der, derlen);
      break;

    case SHISHI_MSGTYPE_AP_REQ:
      node = shishi_der2asn1_apreq (handle, der, derlen);
      break;

    case SHISHI_MSGTYPE_AP_REP:
      node = shishi_der2asn1_aprep (handle, der, derlen);
      break;

    case SHISHI_MSGTYPE_SAFE:
      node = shishi_der2asn1_krbsafe (handle, der, derlen);
      break;

    case SHISHI_MSGTYPE_PRIV:
      node = shishi_der2asn1_priv (handle, der, derlen);
      break;

    case SHISHI_MSGTYPE_CRED:
      /* node = shishi_der2asn1_cred (handle, der, derlen); */
      break;

    case SHISHI_MSGTYPE_ERROR:
      node = shishi_der2asn1_krberror (handle, der, derlen);
      break;

    default:
      node = NULL;
      break;
    }

  return node;
}

/**
 * shishi_der2asn1_ticket:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of Ticket and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_ticket (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.Ticket", "Ticket", der, derlen);
}

/**
 * shishi_der2asn1_encticketpart:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of EncTicketPart and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_encticketpart (Shishi * handle, const char *der,
			       size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.EncTicketPart", "EncTicketPart",
		   der, derlen);
}

/**
 * shishi_der2asn1_asreq:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of AS-REQ and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_asreq (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.AS-REQ", "KDC-REQ", der, derlen);
}

/**
 * shishi_der2asn1_tgsreq:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of TGS-REQ and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_tgsreq (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.TGS-REQ", "KDC-REQ", der, derlen);
}

/**
 * shishi_der2asn1_asrep:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of AS-REP and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_asrep (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.AS-REP", "KDC-REP", der, derlen);
}

/**
 * shishi_der2asn1_tgsrep:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of TGS-REP and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_tgsrep (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.TGS-REP", "KDC-REP", der, derlen);
}

/**
 * shishi_der2asn1_kdcrep:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of KDC-REP and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_kdcrep (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.KDC-REP", "KDC-REP", der, derlen);
}

/**
 * shishi_der2asn1_encasreppart:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of EncASRepPart and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_encasreppart (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.EncASRepPart", "EncKDCRepPart",
		   der, derlen);
}

/**
 * shishi_der2asn1_enctgsreppart:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of EncTGSRepPart and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_enctgsreppart (Shishi * handle, const char *der,
			       size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.EncTGSRepPart", "EncKDCRepPart",
		   der, derlen);
}

/**
 * shishi_der2asn1_enckdcreppart:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of EncKDCRepPart and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_enckdcreppart (Shishi * handle, const char *der,
			       size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.EncKDCRepPart", "EncKDCRepPart",
		   der, derlen);
}

/**
 * shishi_der2asn1_authenticator:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of Authenticator and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_authenticator (Shishi * handle, const char *der,
			       size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.Authenticator", "Authenticator",
		   der, derlen);
}

/**
 * shishi_der2asn1_krberror:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of KRB-ERROR and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_krberror (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.KRB-ERROR", "KRB-ERROR", der, derlen);
}

/**
 * shishi_der2asn1_krbsafe:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of KRB-SAFE and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_krbsafe (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.KRB-SAFE", "KRB-SAFE", der, derlen);
}

/**
 * shishi_der2asn1_priv:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of KRB-PRIV and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_priv (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.KRB-PRIV", "KRB-PRIV", der, derlen);
}

/**
 * shishi_der2asn1_encprivpart:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of EncKrbPrivPart and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_encprivpart (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.EncKrbPrivPart", "EncKrbPrivPart",
		   der, derlen);
}

/**
 * shishi_der2asn1_apreq:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of AP-REQ and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_apreq (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.AP-REQ", "AP-REQ", der, derlen);
}

/**
 * shishi_der2asn1_aprep:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of AP-REP and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_aprep (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.AP-REP", "AP-REP", der, derlen);
}

/**
 * shishi_der2asn1_encapreppart:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of EncAPRepPart and create a ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_encapreppart (Shishi * handle, const char *der, size_t derlen)
{
  return der2asn1 (handle, "Kerberos5.EncAPRepPart", "EncAPRepPart",
		   der, derlen);
}

/**
 * shishi_der2asn1_kdcreq:
 * @handle: shishi handle as allocated by shishi_init().
 * @der: input character array with DER encoding.
 * @derlen: length of input character array with DER encoding.
 *
 * Decode DER encoding of AS-REQ, TGS-REQ or KDC-REQ and create a
 * ASN.1 structure.
 *
 * Return value: Returns ASN.1 structure corresponding to DER data.
 **/
Shishi_asn1
shishi_der2asn1_kdcreq (Shishi * handle, const char *der, size_t derlen)
{
  Shishi_asn1 structure = NULL;

  structure = shishi_der2asn1_asreq (handle, der, derlen);
  if (structure == NULL)
    {
      printf ("der2asn1_kdcreq: not asreq\n");
      shishi_error_printf (handle, "Could not DER decode AS-REQ\n");

      structure = shishi_der2asn1_tgsreq (handle, der, derlen);
      if (structure == NULL)
	{
	  printf ("der2asn1_kdcreq: not tgsreq\n");
	  shishi_error_printf (handle, "Could not DER decode TGS-REQ\n");

	  structure = shishi_der2asn1_kdcreq (handle, der, derlen);
	  if (structure == NULL)
	    {
	      printf ("der2asn1_kdcreq: not kdcreq\n");
	      shishi_error_printf (handle, "Could not DER decode KDC-REQ\n");

	      return NULL;
	    }
	  else
	    printf ("der2asn1_kdcreq: kdcreq!!\n");
	}
    }

  return structure;
}
