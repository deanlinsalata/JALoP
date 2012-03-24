/**
 * @file jalls_xml_utils.hpp This file contains functions to parse and
 * validate xml for the jalop local store.
 *
 * @section LICENSE
 *
 * Source code in 3rd-party is licensed and owned by their respective
 * copyright holders.
 *
 * All other source code is copyright Tresys Technology and licensed as below.
 *
 * Copyright (c) 2012 Tresys Technology LLC, Columbia, Maryland, USA
 *
 * This software was developed by Tresys Technology LLC
 * with U.S. Government sponsorship.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _JALLS_XML_UTILS_HPP_
#define _JALLS_XML_UTILS_HPP_

#define JALLS_XML_APP_META_SCHEMA "applicationMetadata.xsd"
#define JALLS_XML_APP_META_TYPES_SCHEMA "applicationMetadataTypes.xsd"
#define JALLS_XML_SYS_META_SCHEMA "systemMetadata.xsd"
#define JALLS_XML_DSIG_SCHEMA  "xmldsig-core-schema.xsd"
#define JALLS_XML_SCHEMA_DTD "XMLSchema.dtd"
#define JALLS_XML_AUDIT_SCHEMA "cee-cls-xml-event.xsd"

#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <openssl/pem.h>

#include <jalop/jal_status.h>

#include "jalls_handler.h"

XERCES_CPP_NAMESPACE_USE

/**
 * parse and validate the application metadata
 *
 * @param[in] buf A buffer containing the xml data to parse
 * @param[in] size The size of the buffer
 * @param[in] schemas_root The root path where the schemas are located
 * @param[out] doc A pointer to hold the parsed document
 * @param[in] debug A flag for printing debug messages
 */
int jalls_parse_app_metadata(void *buf, size_t size, char* schemas_root, DOMDocument **doc, int debug);

/**
 * parse and validate the audit_data
 *
 * @param[in] buf A buffer containing the xml data to parse
 * @param[in] size The size of the buffer
 * @param[in] schemas_root The root path where the schemas are located
 * @param[out] doc A pointer to hold the parsed document
 * @param[in] debug A flag for printing debug messages
 */
int jalls_parse_audit(void *buf, size_t size, char *schemas_root, DOMDocument **doc, int debug);

/**
 * Helper function to base64 encode a buffer and create a new DOMElement.
 *
 * @param[in] doc The document to use when creating elements.
 * @param[in] buffer The byte buffer to base64 encode
 * @param[in] buf_len The length, in bytes, of the buffer
 * @param[in] namespace_uri The URI to use as the namespace of the new element.
 * @param[in] elm_name The name that should be given to the new element.
 * @param[in,out] new_elem Pointer that will be assigned to the newly created
 * DOMElement, This new DOMElement will have the name
 * \pelm_name, and the default namespace set to \pnamespace_uri. The text
 * content of the new node will be the base64 encoded value of \pbuffer.
 *
 * @return JAL_OK on error, or JAL_E_INVAL
 */
enum jal_status jalls_create_base64_element(DOMDocument *doc,
		const uint8_t *buffer, const size_t buf_len,
		const XMLCh *namespace_uri, const XMLCh *elm_name,
		DOMElement **new_elem);

/**
 * Helper function to create a properly formatted XML DateTime timestamp.
 *
 * @return formatted timestamp string
 */
char *jalls_get_timestamp();

/**
 * Creates Reference DOMElement from uri, digest method, and digest buffer
 *
 * Does not deal with the transfrom child Element, this will be appended later by the caller.
 *
 * @param[in] reference_uri A uri for the reference
 * @param[in] digest_method A uri for the algorithm used in creating the digest.
 * @param[in] digest_buf A pointer to the generated digest.
 * @param[in] digest_len The length of the generated digest.
 * @param[in] doc A pointer to the DOMDocument to use in creating the element
 * @param[out] elem A pointer to hold the created element. Should point to NULL.
 * @return JAL_OK on success, JAL_E_XML_CONVERSION on failure.
*/
enum jal_status jalls_create_reference_elem(const char *reference_uri, const char *digest_method,
		uint8_t *digest_buf, size_t len,
		DOMDocument *doc, DOMElement **elem);


/**
 * Use the digest context \pdgst_ctx to generate a digest for the document
 * given by xml_buffer.
 *
 * @param dgst_ctx The digest method to use.
 * @param doc The DOM Document to generate a digest for.
 * @param digest_out On success, this will be set to a newly allocated buffer
 * that contains the binary version of the digest. It is up to the caller to
 * release this memory with a call to free().
 * @param digest_len On success, this will be set to the length, in bytes, of
 * \pdigest_buffer.
 * @return JAL_OK on success, or an error code.
 */
enum jal_status jalls_digest_xml_data(const struct jal_digest_ctx *dgst_ctx,
		XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc,
		uint8_t **digest_out,
		int *digest_len);

/**
 * Helper function return a Transforms element.
 *
 * @param[in] doc The document to use when creating elements.
 * @param[in,out] new_elem Pointer that will be assigned to the newly created
 * Transforms element.
 *
 * @return JAL_OK,  or JAL_E_XML_CONVERSION on error
 */
enum jal_status jalls_create_audit_transforms_elem(DOMDocument *doc, DOMElement **new_elem);

/**
 * Given a DOMDocument, write out the corresponding XML to a byte buffer.
 *
 * @param doc[in] The DOMDocument to serialize
 * @param buffer[out] The buffer that contains the serialized XML.
 * @return 
 *  - JAL_OK on success
 *  - JAL_E_XML_CONVERSION if there is an error serializing the document
 *  - JAL_E_INVAL if one of the arguments is invalid.
 */
enum jal_status jalls_xml_output(DOMDocument *doc, MemBufFormatTarget **buffer);

/**
 * Convert an OpenSSL BIGNUM to a Xerces XMLCh pointer of the decimal representation
 * of the BIGNUM.
 *
 * @param[in] bn The BIGNUM to convert.
 *
 * @return XMLCh pointer to the decimal representation of the BIGNUM.  This needs
 * to be free'd with XMLString::release().
 */
XMLCh *jalls_BN2decXMLCh(BIGNUM *bn);

/**
 * Convert a certificate name to a XMLCh pointer.
 *
 * @param[in] nm Certificate subject.
 *
 * @return Certificate subject name as a XMLCh pointer.
 */
XMLCh *jalls_get_xml_x509_name(X509_NAME *nm);

/**
 * Convert a certificate serial number to a XMLCh pointer.
 *
 * @param[in] i ASN1_INTEGER certificate serial number.
 *
 * @return Certificate serial number as a XMLCh pointer.
 */
XMLCh *jalls_get_xml_x509_serial(ASN1_INTEGER *i);

/**
 * Convert a base64-encoded certificate to a XMLCh pointer.
 *
 * @param[in] x509 The certificate.
 *
 * @return Certificate as a XMLCh pointer.
 */
XMLCh *jalls_get_xml_x509_cert(X509 *x509);

/**
 * Convert an OpenSSL BIGNUM to a Xerces XMLCh pointer of the base 64 representation
 * of the BIGNUM.
 *
 * @param[in] bn The BIGNUM to convert.
 *
 * @return XMLCh pointer to the b64 representation of the BIGNUM.  This needs
 * to be free'd with XMLString::release().
 */
XMLCh *jalls_BN2b64(BIGNUM *bn);

/**
 * Add a signature block to a document.
 *
 * @param[in] rsa RSA key to use as a signing key.  This is required.
 * @param[in] x509 Certificate to use when signing.  This is not required
 * and could be passed in as NULL.
 * @param[in] doc The document to sign.
 * @param[in] parent_element The element to add the signature to. This will
 * probably be the document's root element.
 * @param[in] last_element The element to add the signature before.  Pass in
 * NULL to have the signature element added as the last element under
 * parent_element.
 * @param[in] id The id to use in the Reference elements URI attribute.
 * This should be the id of the root node in the document.
 *
 * @return JAL_OK,  or JAL_E_INVAL on error
 */
enum jal_status jalls_add_signature_block(RSA *rsa, X509 *x509, DOMDocument *doc,
		DOMElement *parent_element, DOMElement *last_element, const XMLCh *id);


#endif //_JALLS_XML_UTILS_HPP_
