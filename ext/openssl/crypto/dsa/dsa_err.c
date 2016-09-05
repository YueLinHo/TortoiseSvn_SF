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
#include <openssl/dsa.h>

/* BEGIN ERROR CODES */
#ifndef OPENSSL_NO_ERR

# define ERR_FUNC(func) ERR_PACK(ERR_LIB_DSA,func,0)
# define ERR_REASON(reason) ERR_PACK(ERR_LIB_DSA,0,reason)

static ERR_STRING_DATA DSA_str_functs[] = {
    {ERR_FUNC(DSA_F_DSAPARAMS_PRINT), "DSAparams_print"},
    {ERR_FUNC(DSA_F_DSAPARAMS_PRINT_FP), "DSAparams_print_fp"},
    {ERR_FUNC(DSA_F_DSA_BUILTIN_PARAMGEN), "DSA_BUILTIN_PARAMGEN"},
    {ERR_FUNC(DSA_F_DSA_BUILTIN_PARAMGEN2), "dsa_builtin_paramgen2"},
    {ERR_FUNC(DSA_F_DSA_DO_SIGN), "DSA_do_sign"},
    {ERR_FUNC(DSA_F_DSA_DO_VERIFY), "DSA_do_verify"},
    {ERR_FUNC(DSA_F_DSA_METH_DUP), "DSA_meth_dup"},
    {ERR_FUNC(DSA_F_DSA_METH_NEW), "DSA_meth_new"},
    {ERR_FUNC(DSA_F_DSA_METH_SET1_NAME), "DSA_meth_set1_name"},
    {ERR_FUNC(DSA_F_DSA_NEW_METHOD), "DSA_new_method"},
    {ERR_FUNC(DSA_F_DSA_PARAM_DECODE), "dsa_param_decode"},
    {ERR_FUNC(DSA_F_DSA_PRINT_FP), "DSA_print_fp"},
    {ERR_FUNC(DSA_F_DSA_PRIV_DECODE), "dsa_priv_decode"},
    {ERR_FUNC(DSA_F_DSA_PRIV_ENCODE), "dsa_priv_encode"},
    {ERR_FUNC(DSA_F_DSA_PUB_DECODE), "dsa_pub_decode"},
    {ERR_FUNC(DSA_F_DSA_PUB_ENCODE), "dsa_pub_encode"},
    {ERR_FUNC(DSA_F_DSA_SIGN), "DSA_sign"},
    {ERR_FUNC(DSA_F_DSA_SIGN_SETUP), "DSA_sign_setup"},
    {ERR_FUNC(DSA_F_DSA_SIG_NEW), "DSA_SIG_new"},
    {ERR_FUNC(DSA_F_OLD_DSA_PRIV_DECODE), "old_dsa_priv_decode"},
    {ERR_FUNC(DSA_F_PKEY_DSA_CTRL), "pkey_dsa_ctrl"},
    {ERR_FUNC(DSA_F_PKEY_DSA_KEYGEN), "pkey_dsa_keygen"},
    {0, NULL}
};

static ERR_STRING_DATA DSA_str_reasons[] = {
    {ERR_REASON(DSA_R_BAD_Q_VALUE), "bad q value"},
    {ERR_REASON(DSA_R_BN_DECODE_ERROR), "bn decode error"},
    {ERR_REASON(DSA_R_BN_ERROR), "bn error"},
    {ERR_REASON(DSA_R_DECODE_ERROR), "decode error"},
    {ERR_REASON(DSA_R_INVALID_DIGEST_TYPE), "invalid digest type"},
    {ERR_REASON(DSA_R_INVALID_PARAMETERS), "invalid parameters"},
    {ERR_REASON(DSA_R_MISSING_PARAMETERS), "missing parameters"},
    {ERR_REASON(DSA_R_MODULUS_TOO_LARGE), "modulus too large"},
    {ERR_REASON(DSA_R_NO_PARAMETERS_SET), "no parameters set"},
    {ERR_REASON(DSA_R_PARAMETER_ENCODING_ERROR), "parameter encoding error"},
    {ERR_REASON(DSA_R_Q_NOT_PRIME), "q not prime"},
    {0, NULL}
};

#endif

int ERR_load_DSA_strings(void)
{
#ifndef OPENSSL_NO_ERR

    if (ERR_func_error_string(DSA_str_functs[0].error) == NULL) {
        ERR_load_strings(0, DSA_str_functs);
        ERR_load_strings(0, DSA_str_reasons);
    }
#endif
    return 1;
}
