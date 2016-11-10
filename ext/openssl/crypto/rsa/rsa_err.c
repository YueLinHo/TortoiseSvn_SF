/*
 * Generated by util/mkerr.pl DO NOT EDIT
 * Copyright 1995-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include <stdio.h>
#include <openssl/err.h>
#include <openssl/rsa.h>

/* BEGIN ERROR CODES */
#ifndef OPENSSL_NO_ERR

# define ERR_FUNC(func) ERR_PACK(ERR_LIB_RSA,func,0)
# define ERR_REASON(reason) ERR_PACK(ERR_LIB_RSA,0,reason)

static ERR_STRING_DATA RSA_str_functs[] = {
    {ERR_FUNC(RSA_F_CHECK_PADDING_MD), "check_padding_md"},
    {ERR_FUNC(RSA_F_ENCODE_PKCS1), "encode_pkcs1"},
    {ERR_FUNC(RSA_F_INT_RSA_VERIFY), "int_rsa_verify"},
    {ERR_FUNC(RSA_F_OLD_RSA_PRIV_DECODE), "old_rsa_priv_decode"},
    {ERR_FUNC(RSA_F_PKEY_RSA_CTRL), "pkey_rsa_ctrl"},
    {ERR_FUNC(RSA_F_PKEY_RSA_CTRL_STR), "pkey_rsa_ctrl_str"},
    {ERR_FUNC(RSA_F_PKEY_RSA_SIGN), "pkey_rsa_sign"},
    {ERR_FUNC(RSA_F_PKEY_RSA_VERIFYRECOVER), "pkey_rsa_verifyrecover"},
    {ERR_FUNC(RSA_F_RSA_ALGOR_TO_MD), "rsa_algor_to_md"},
    {ERR_FUNC(RSA_F_RSA_BUILTIN_KEYGEN), "rsa_builtin_keygen"},
    {ERR_FUNC(RSA_F_RSA_CHECK_KEY), "RSA_check_key"},
    {ERR_FUNC(RSA_F_RSA_CHECK_KEY_EX), "RSA_check_key_ex"},
    {ERR_FUNC(RSA_F_RSA_CMS_DECRYPT), "rsa_cms_decrypt"},
    {ERR_FUNC(RSA_F_RSA_ITEM_VERIFY), "rsa_item_verify"},
    {ERR_FUNC(RSA_F_RSA_METH_DUP), "RSA_meth_dup"},
    {ERR_FUNC(RSA_F_RSA_METH_NEW), "RSA_meth_new"},
    {ERR_FUNC(RSA_F_RSA_METH_SET1_NAME), "RSA_meth_set1_name"},
    {ERR_FUNC(RSA_F_RSA_MGF1_TO_MD), "rsa_mgf1_to_md"},
    {ERR_FUNC(RSA_F_RSA_NEW_METHOD), "RSA_new_method"},
    {ERR_FUNC(RSA_F_RSA_NULL), "RSA_NULL"},
    {ERR_FUNC(RSA_F_RSA_NULL_PRIVATE_DECRYPT), "RSA_null_private_decrypt"},
    {ERR_FUNC(RSA_F_RSA_NULL_PRIVATE_ENCRYPT), "RSA_null_private_encrypt"},
    {ERR_FUNC(RSA_F_RSA_NULL_PUBLIC_DECRYPT), "RSA_null_public_decrypt"},
    {ERR_FUNC(RSA_F_RSA_NULL_PUBLIC_ENCRYPT), "RSA_null_public_encrypt"},
    {ERR_FUNC(RSA_F_RSA_OSSL_PRIVATE_DECRYPT), "rsa_ossl_private_decrypt"},
    {ERR_FUNC(RSA_F_RSA_OSSL_PRIVATE_ENCRYPT), "rsa_ossl_private_encrypt"},
    {ERR_FUNC(RSA_F_RSA_OSSL_PUBLIC_DECRYPT), "rsa_ossl_public_decrypt"},
    {ERR_FUNC(RSA_F_RSA_OSSL_PUBLIC_ENCRYPT), "rsa_ossl_public_encrypt"},
    {ERR_FUNC(RSA_F_RSA_PADDING_ADD_NONE), "RSA_padding_add_none"},
    {ERR_FUNC(RSA_F_RSA_PADDING_ADD_PKCS1_OAEP),
     "RSA_padding_add_PKCS1_OAEP"},
    {ERR_FUNC(RSA_F_RSA_PADDING_ADD_PKCS1_OAEP_MGF1),
     "RSA_padding_add_PKCS1_OAEP_mgf1"},
    {ERR_FUNC(RSA_F_RSA_PADDING_ADD_PKCS1_PSS), "RSA_padding_add_PKCS1_PSS"},
    {ERR_FUNC(RSA_F_RSA_PADDING_ADD_PKCS1_PSS_MGF1),
     "RSA_padding_add_PKCS1_PSS_mgf1"},
    {ERR_FUNC(RSA_F_RSA_PADDING_ADD_PKCS1_TYPE_1),
     "RSA_padding_add_PKCS1_type_1"},
    {ERR_FUNC(RSA_F_RSA_PADDING_ADD_PKCS1_TYPE_2),
     "RSA_padding_add_PKCS1_type_2"},
    {ERR_FUNC(RSA_F_RSA_PADDING_ADD_SSLV23), "RSA_padding_add_SSLv23"},
    {ERR_FUNC(RSA_F_RSA_PADDING_ADD_X931), "RSA_padding_add_X931"},
    {ERR_FUNC(RSA_F_RSA_PADDING_CHECK_NONE), "RSA_padding_check_none"},
    {ERR_FUNC(RSA_F_RSA_PADDING_CHECK_PKCS1_OAEP),
     "RSA_padding_check_PKCS1_OAEP"},
    {ERR_FUNC(RSA_F_RSA_PADDING_CHECK_PKCS1_OAEP_MGF1),
     "RSA_padding_check_PKCS1_OAEP_mgf1"},
    {ERR_FUNC(RSA_F_RSA_PADDING_CHECK_PKCS1_TYPE_1),
     "RSA_padding_check_PKCS1_type_1"},
    {ERR_FUNC(RSA_F_RSA_PADDING_CHECK_PKCS1_TYPE_2),
     "RSA_padding_check_PKCS1_type_2"},
    {ERR_FUNC(RSA_F_RSA_PADDING_CHECK_SSLV23), "RSA_padding_check_SSLv23"},
    {ERR_FUNC(RSA_F_RSA_PADDING_CHECK_X931), "RSA_padding_check_X931"},
    {ERR_FUNC(RSA_F_RSA_PRINT), "RSA_print"},
    {ERR_FUNC(RSA_F_RSA_PRINT_FP), "RSA_print_fp"},
    {ERR_FUNC(RSA_F_RSA_PRIV_ENCODE), "rsa_priv_encode"},
    {ERR_FUNC(RSA_F_RSA_PSS_TO_CTX), "rsa_pss_to_ctx"},
    {ERR_FUNC(RSA_F_RSA_PUB_DECODE), "rsa_pub_decode"},
    {ERR_FUNC(RSA_F_RSA_SETUP_BLINDING), "RSA_setup_blinding"},
    {ERR_FUNC(RSA_F_RSA_SIGN), "RSA_sign"},
    {ERR_FUNC(RSA_F_RSA_SIGN_ASN1_OCTET_STRING),
     "RSA_sign_ASN1_OCTET_STRING"},
    {ERR_FUNC(RSA_F_RSA_VERIFY), "RSA_verify"},
    {ERR_FUNC(RSA_F_RSA_VERIFY_ASN1_OCTET_STRING),
     "RSA_verify_ASN1_OCTET_STRING"},
    {ERR_FUNC(RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1), "RSA_verify_PKCS1_PSS_mgf1"},
    {0, NULL}
};

static ERR_STRING_DATA RSA_str_reasons[] = {
    {ERR_REASON(RSA_R_ALGORITHM_MISMATCH), "algorithm mismatch"},
    {ERR_REASON(RSA_R_BAD_E_VALUE), "bad e value"},
    {ERR_REASON(RSA_R_BAD_FIXED_HEADER_DECRYPT), "bad fixed header decrypt"},
    {ERR_REASON(RSA_R_BAD_PAD_BYTE_COUNT), "bad pad byte count"},
    {ERR_REASON(RSA_R_BAD_SIGNATURE), "bad signature"},
    {ERR_REASON(RSA_R_BLOCK_TYPE_IS_NOT_01), "block type is not 01"},
    {ERR_REASON(RSA_R_BLOCK_TYPE_IS_NOT_02), "block type is not 02"},
    {ERR_REASON(RSA_R_DATA_GREATER_THAN_MOD_LEN),
     "data greater than mod len"},
    {ERR_REASON(RSA_R_DATA_TOO_LARGE), "data too large"},
    {ERR_REASON(RSA_R_DATA_TOO_LARGE_FOR_KEY_SIZE),
     "data too large for key size"},
    {ERR_REASON(RSA_R_DATA_TOO_LARGE_FOR_MODULUS),
     "data too large for modulus"},
    {ERR_REASON(RSA_R_DATA_TOO_SMALL), "data too small"},
    {ERR_REASON(RSA_R_DATA_TOO_SMALL_FOR_KEY_SIZE),
     "data too small for key size"},
    {ERR_REASON(RSA_R_DIGEST_DOES_NOT_MATCH), "digest does not match"},
    {ERR_REASON(RSA_R_DIGEST_TOO_BIG_FOR_RSA_KEY),
     "digest too big for rsa key"},
    {ERR_REASON(RSA_R_DMP1_NOT_CONGRUENT_TO_D), "dmp1 not congruent to d"},
    {ERR_REASON(RSA_R_DMQ1_NOT_CONGRUENT_TO_D), "dmq1 not congruent to d"},
    {ERR_REASON(RSA_R_D_E_NOT_CONGRUENT_TO_1), "d e not congruent to 1"},
    {ERR_REASON(RSA_R_FIRST_OCTET_INVALID), "first octet invalid"},
    {ERR_REASON(RSA_R_ILLEGAL_OR_UNSUPPORTED_PADDING_MODE),
     "illegal or unsupported padding mode"},
    {ERR_REASON(RSA_R_INVALID_DIGEST), "invalid digest"},
    {ERR_REASON(RSA_R_INVALID_DIGEST_LENGTH), "invalid digest length"},
    {ERR_REASON(RSA_R_INVALID_HEADER), "invalid header"},
    {ERR_REASON(RSA_R_INVALID_LABEL), "invalid label"},
    {ERR_REASON(RSA_R_INVALID_MESSAGE_LENGTH), "invalid message length"},
    {ERR_REASON(RSA_R_INVALID_MGF1_MD), "invalid mgf1 md"},
    {ERR_REASON(RSA_R_INVALID_OAEP_PARAMETERS), "invalid oaep parameters"},
    {ERR_REASON(RSA_R_INVALID_PADDING), "invalid padding"},
    {ERR_REASON(RSA_R_INVALID_PADDING_MODE), "invalid padding mode"},
    {ERR_REASON(RSA_R_INVALID_PSS_PARAMETERS), "invalid pss parameters"},
    {ERR_REASON(RSA_R_INVALID_PSS_SALTLEN), "invalid pss saltlen"},
    {ERR_REASON(RSA_R_INVALID_SALT_LENGTH), "invalid salt length"},
    {ERR_REASON(RSA_R_INVALID_TRAILER), "invalid trailer"},
    {ERR_REASON(RSA_R_INVALID_X931_DIGEST), "invalid x931 digest"},
    {ERR_REASON(RSA_R_IQMP_NOT_INVERSE_OF_Q), "iqmp not inverse of q"},
    {ERR_REASON(RSA_R_KEY_SIZE_TOO_SMALL), "key size too small"},
    {ERR_REASON(RSA_R_LAST_OCTET_INVALID), "last octet invalid"},
    {ERR_REASON(RSA_R_MODULUS_TOO_LARGE), "modulus too large"},
    {ERR_REASON(RSA_R_NO_PUBLIC_EXPONENT), "no public exponent"},
    {ERR_REASON(RSA_R_NULL_BEFORE_BLOCK_MISSING),
     "null before block missing"},
    {ERR_REASON(RSA_R_N_DOES_NOT_EQUAL_P_Q), "n does not equal p q"},
    {ERR_REASON(RSA_R_OAEP_DECODING_ERROR), "oaep decoding error"},
    {ERR_REASON(RSA_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE),
     "operation not supported for this keytype"},
    {ERR_REASON(RSA_R_PADDING_CHECK_FAILED), "padding check failed"},
    {ERR_REASON(RSA_R_PKCS_DECODING_ERROR), "pkcs decoding error"},
    {ERR_REASON(RSA_R_P_NOT_PRIME), "p not prime"},
    {ERR_REASON(RSA_R_Q_NOT_PRIME), "q not prime"},
    {ERR_REASON(RSA_R_RSA_OPERATIONS_NOT_SUPPORTED),
     "rsa operations not supported"},
    {ERR_REASON(RSA_R_SLEN_CHECK_FAILED), "salt length check failed"},
    {ERR_REASON(RSA_R_SLEN_RECOVERY_FAILED), "salt length recovery failed"},
    {ERR_REASON(RSA_R_SSLV3_ROLLBACK_ATTACK), "sslv3 rollback attack"},
    {ERR_REASON(RSA_R_THE_ASN1_OBJECT_IDENTIFIER_IS_NOT_KNOWN_FOR_THIS_MD),
     "the asn1 object identifier is not known for this md"},
    {ERR_REASON(RSA_R_UNKNOWN_ALGORITHM_TYPE), "unknown algorithm type"},
    {ERR_REASON(RSA_R_UNKNOWN_DIGEST), "unknown digest"},
    {ERR_REASON(RSA_R_UNKNOWN_MASK_DIGEST), "unknown mask digest"},
    {ERR_REASON(RSA_R_UNKNOWN_PADDING_TYPE), "unknown padding type"},
    {ERR_REASON(RSA_R_UNSUPPORTED_ENCRYPTION_TYPE),
     "unsupported encryption type"},
    {ERR_REASON(RSA_R_UNSUPPORTED_LABEL_SOURCE), "unsupported label source"},
    {ERR_REASON(RSA_R_UNSUPPORTED_MASK_ALGORITHM),
     "unsupported mask algorithm"},
    {ERR_REASON(RSA_R_UNSUPPORTED_MASK_PARAMETER),
     "unsupported mask parameter"},
    {ERR_REASON(RSA_R_UNSUPPORTED_SIGNATURE_TYPE),
     "unsupported signature type"},
    {ERR_REASON(RSA_R_VALUE_MISSING), "value missing"},
    {ERR_REASON(RSA_R_WRONG_SIGNATURE_LENGTH), "wrong signature length"},
    {0, NULL}
};

#endif

int ERR_load_RSA_strings(void)
{
#ifndef OPENSSL_NO_ERR

    if (ERR_func_error_string(RSA_str_functs[0].error) == NULL) {
        ERR_load_strings(0, RSA_str_functs);
        ERR_load_strings(0, RSA_str_reasons);
    }
#endif
    return 1;
}
