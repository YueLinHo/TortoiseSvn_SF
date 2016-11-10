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
#include <openssl/ssl.h>

/* BEGIN ERROR CODES */
#ifndef OPENSSL_NO_ERR

# define ERR_FUNC(func) ERR_PACK(ERR_LIB_SSL,func,0)
# define ERR_REASON(reason) ERR_PACK(ERR_LIB_SSL,0,reason)

static ERR_STRING_DATA SSL_str_functs[] = {
    {ERR_FUNC(SSL_F_CHECK_SUITEB_CIPHER_LIST), "check_suiteb_cipher_list"},
    {ERR_FUNC(SSL_F_CT_MOVE_SCTS), "ct_move_scts"},
    {ERR_FUNC(SSL_F_CT_STRICT), "ct_strict"},
    {ERR_FUNC(SSL_F_D2I_SSL_SESSION), "d2i_SSL_SESSION"},
    {ERR_FUNC(SSL_F_DANE_CTX_ENABLE), "dane_ctx_enable"},
    {ERR_FUNC(SSL_F_DANE_MTYPE_SET), "dane_mtype_set"},
    {ERR_FUNC(SSL_F_DANE_TLSA_ADD), "dane_tlsa_add"},
    {ERR_FUNC(SSL_F_DO_DTLS1_WRITE), "do_dtls1_write"},
    {ERR_FUNC(SSL_F_DO_SSL3_WRITE), "do_ssl3_write"},
    {ERR_FUNC(SSL_F_DTLS1_BUFFER_RECORD), "dtls1_buffer_record"},
    {ERR_FUNC(SSL_F_DTLS1_CHECK_TIMEOUT_NUM), "dtls1_check_timeout_num"},
    {ERR_FUNC(SSL_F_DTLS1_HEARTBEAT), "dtls1_heartbeat"},
    {ERR_FUNC(SSL_F_DTLS1_PREPROCESS_FRAGMENT), "dtls1_preprocess_fragment"},
    {ERR_FUNC(SSL_F_DTLS1_PROCESS_BUFFERED_RECORDS),
     "dtls1_process_buffered_records"},
    {ERR_FUNC(SSL_F_DTLS1_PROCESS_RECORD), "dtls1_process_record"},
    {ERR_FUNC(SSL_F_DTLS1_READ_BYTES), "dtls1_read_bytes"},
    {ERR_FUNC(SSL_F_DTLS1_READ_FAILED), "dtls1_read_failed"},
    {ERR_FUNC(SSL_F_DTLS1_RETRANSMIT_MESSAGE), "dtls1_retransmit_message"},
    {ERR_FUNC(SSL_F_DTLS1_WRITE_APP_DATA_BYTES),
     "dtls1_write_app_data_bytes"},
    {ERR_FUNC(SSL_F_DTLSV1_LISTEN), "DTLSv1_listen"},
    {ERR_FUNC(SSL_F_DTLS_CONSTRUCT_CHANGE_CIPHER_SPEC),
     "dtls_construct_change_cipher_spec"},
    {ERR_FUNC(SSL_F_DTLS_CONSTRUCT_HELLO_VERIFY_REQUEST),
     "dtls_construct_hello_verify_request"},
    {ERR_FUNC(SSL_F_DTLS_GET_REASSEMBLED_MESSAGE),
     "dtls_get_reassembled_message"},
    {ERR_FUNC(SSL_F_DTLS_PROCESS_HELLO_VERIFY), "dtls_process_hello_verify"},
    {ERR_FUNC(SSL_F_OPENSSL_INIT_SSL), "OPENSSL_init_ssl"},
    {ERR_FUNC(SSL_F_OSSL_STATEM_CLIENT_READ_TRANSITION),
     "ossl_statem_client_read_transition"},
    {ERR_FUNC(SSL_F_OSSL_STATEM_SERVER_READ_TRANSITION),
     "ossl_statem_server_read_transition"},
    {ERR_FUNC(SSL_F_READ_STATE_MACHINE), "read_state_machine"},
    {ERR_FUNC(SSL_F_SSL3_CHANGE_CIPHER_STATE), "ssl3_change_cipher_state"},
    {ERR_FUNC(SSL_F_SSL3_CHECK_CERT_AND_ALGORITHM),
     "ssl3_check_cert_and_algorithm"},
    {ERR_FUNC(SSL_F_SSL3_CTRL), "ssl3_ctrl"},
    {ERR_FUNC(SSL_F_SSL3_CTX_CTRL), "ssl3_ctx_ctrl"},
    {ERR_FUNC(SSL_F_SSL3_DIGEST_CACHED_RECORDS),
     "ssl3_digest_cached_records"},
    {ERR_FUNC(SSL_F_SSL3_DO_CHANGE_CIPHER_SPEC),
     "ssl3_do_change_cipher_spec"},
    {ERR_FUNC(SSL_F_SSL3_FINAL_FINISH_MAC), "ssl3_final_finish_mac"},
    {ERR_FUNC(SSL_F_SSL3_GENERATE_KEY_BLOCK), "ssl3_generate_key_block"},
    {ERR_FUNC(SSL_F_SSL3_GENERATE_MASTER_SECRET),
     "ssl3_generate_master_secret"},
    {ERR_FUNC(SSL_F_SSL3_GET_RECORD), "ssl3_get_record"},
    {ERR_FUNC(SSL_F_SSL3_INIT_FINISHED_MAC), "ssl3_init_finished_mac"},
    {ERR_FUNC(SSL_F_SSL3_OUTPUT_CERT_CHAIN), "ssl3_output_cert_chain"},
    {ERR_FUNC(SSL_F_SSL3_READ_BYTES), "ssl3_read_bytes"},
    {ERR_FUNC(SSL_F_SSL3_READ_N), "ssl3_read_n"},
    {ERR_FUNC(SSL_F_SSL3_SETUP_KEY_BLOCK), "ssl3_setup_key_block"},
    {ERR_FUNC(SSL_F_SSL3_SETUP_READ_BUFFER), "ssl3_setup_read_buffer"},
    {ERR_FUNC(SSL_F_SSL3_SETUP_WRITE_BUFFER), "ssl3_setup_write_buffer"},
    {ERR_FUNC(SSL_F_SSL3_WRITE_BYTES), "ssl3_write_bytes"},
    {ERR_FUNC(SSL_F_SSL3_WRITE_PENDING), "ssl3_write_pending"},
    {ERR_FUNC(SSL_F_SSL_ADD_CERT_CHAIN), "ssl_add_cert_chain"},
    {ERR_FUNC(SSL_F_SSL_ADD_CERT_TO_BUF), "ssl_add_cert_to_buf"},
    {ERR_FUNC(SSL_F_SSL_ADD_CLIENTHELLO_RENEGOTIATE_EXT),
     "ssl_add_clienthello_renegotiate_ext"},
    {ERR_FUNC(SSL_F_SSL_ADD_CLIENTHELLO_TLSEXT),
     "ssl_add_clienthello_tlsext"},
    {ERR_FUNC(SSL_F_SSL_ADD_CLIENTHELLO_USE_SRTP_EXT),
     "ssl_add_clienthello_use_srtp_ext"},
    {ERR_FUNC(SSL_F_SSL_ADD_DIR_CERT_SUBJECTS_TO_STACK),
     "SSL_add_dir_cert_subjects_to_stack"},
    {ERR_FUNC(SSL_F_SSL_ADD_FILE_CERT_SUBJECTS_TO_STACK),
     "SSL_add_file_cert_subjects_to_stack"},
    {ERR_FUNC(SSL_F_SSL_ADD_SERVERHELLO_RENEGOTIATE_EXT),
     "ssl_add_serverhello_renegotiate_ext"},
    {ERR_FUNC(SSL_F_SSL_ADD_SERVERHELLO_TLSEXT),
     "ssl_add_serverhello_tlsext"},
    {ERR_FUNC(SSL_F_SSL_ADD_SERVERHELLO_USE_SRTP_EXT),
     "ssl_add_serverhello_use_srtp_ext"},
    {ERR_FUNC(SSL_F_SSL_BAD_METHOD), "ssl_bad_method"},
    {ERR_FUNC(SSL_F_SSL_BUILD_CERT_CHAIN), "ssl_build_cert_chain"},
    {ERR_FUNC(SSL_F_SSL_BYTES_TO_CIPHER_LIST), "ssl_bytes_to_cipher_list"},
    {ERR_FUNC(SSL_F_SSL_CERT_ADD0_CHAIN_CERT), "ssl_cert_add0_chain_cert"},
    {ERR_FUNC(SSL_F_SSL_CERT_DUP), "ssl_cert_dup"},
    {ERR_FUNC(SSL_F_SSL_CERT_NEW), "ssl_cert_new"},
    {ERR_FUNC(SSL_F_SSL_CERT_SET0_CHAIN), "ssl_cert_set0_chain"},
    {ERR_FUNC(SSL_F_SSL_CHECK_PRIVATE_KEY), "SSL_check_private_key"},
    {ERR_FUNC(SSL_F_SSL_CHECK_SERVERHELLO_TLSEXT),
     "ssl_check_serverhello_tlsext"},
    {ERR_FUNC(SSL_F_SSL_CHECK_SRVR_ECC_CERT_AND_ALG),
     "ssl_check_srvr_ecc_cert_and_alg"},
    {ERR_FUNC(SSL_F_SSL_CIPHER_PROCESS_RULESTR),
     "ssl_cipher_process_rulestr"},
    {ERR_FUNC(SSL_F_SSL_CIPHER_STRENGTH_SORT), "ssl_cipher_strength_sort"},
    {ERR_FUNC(SSL_F_SSL_CLEAR), "SSL_clear"},
    {ERR_FUNC(SSL_F_SSL_COMP_ADD_COMPRESSION_METHOD),
     "SSL_COMP_add_compression_method"},
    {ERR_FUNC(SSL_F_SSL_CONF_CMD), "SSL_CONF_cmd"},
    {ERR_FUNC(SSL_F_SSL_CREATE_CIPHER_LIST), "ssl_create_cipher_list"},
    {ERR_FUNC(SSL_F_SSL_CTRL), "SSL_ctrl"},
    {ERR_FUNC(SSL_F_SSL_CTX_CHECK_PRIVATE_KEY), "SSL_CTX_check_private_key"},
    {ERR_FUNC(SSL_F_SSL_CTX_ENABLE_CT), "SSL_CTX_enable_ct"},
    {ERR_FUNC(SSL_F_SSL_CTX_MAKE_PROFILES), "ssl_ctx_make_profiles"},
    {ERR_FUNC(SSL_F_SSL_CTX_NEW), "SSL_CTX_new"},
    {ERR_FUNC(SSL_F_SSL_CTX_SET_ALPN_PROTOS), "SSL_CTX_set_alpn_protos"},
    {ERR_FUNC(SSL_F_SSL_CTX_SET_CIPHER_LIST), "SSL_CTX_set_cipher_list"},
    {ERR_FUNC(SSL_F_SSL_CTX_SET_CLIENT_CERT_ENGINE),
     "SSL_CTX_set_client_cert_engine"},
    {ERR_FUNC(SSL_F_SSL_CTX_SET_CT_VALIDATION_CALLBACK),
     "SSL_CTX_set_ct_validation_callback"},
    {ERR_FUNC(SSL_F_SSL_CTX_SET_SESSION_ID_CONTEXT),
     "SSL_CTX_set_session_id_context"},
    {ERR_FUNC(SSL_F_SSL_CTX_SET_SSL_VERSION), "SSL_CTX_set_ssl_version"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_CERTIFICATE), "SSL_CTX_use_certificate"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_CERTIFICATE_ASN1),
     "SSL_CTX_use_certificate_ASN1"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_CERTIFICATE_FILE),
     "SSL_CTX_use_certificate_file"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_PRIVATEKEY), "SSL_CTX_use_PrivateKey"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_PRIVATEKEY_ASN1),
     "SSL_CTX_use_PrivateKey_ASN1"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_PRIVATEKEY_FILE),
     "SSL_CTX_use_PrivateKey_file"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_PSK_IDENTITY_HINT),
     "SSL_CTX_use_psk_identity_hint"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_RSAPRIVATEKEY), "SSL_CTX_use_RSAPrivateKey"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_RSAPRIVATEKEY_ASN1),
     "SSL_CTX_use_RSAPrivateKey_ASN1"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_RSAPRIVATEKEY_FILE),
     "SSL_CTX_use_RSAPrivateKey_file"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_SERVERINFO), "SSL_CTX_use_serverinfo"},
    {ERR_FUNC(SSL_F_SSL_CTX_USE_SERVERINFO_FILE),
     "SSL_CTX_use_serverinfo_file"},
    {ERR_FUNC(SSL_F_SSL_DANE_DUP), "ssl_dane_dup"},
    {ERR_FUNC(SSL_F_SSL_DANE_ENABLE), "SSL_dane_enable"},
    {ERR_FUNC(SSL_F_SSL_DO_CONFIG), "ssl_do_config"},
    {ERR_FUNC(SSL_F_SSL_DO_HANDSHAKE), "SSL_do_handshake"},
    {ERR_FUNC(SSL_F_SSL_DUP_CA_LIST), "SSL_dup_CA_list"},
    {ERR_FUNC(SSL_F_SSL_ENABLE_CT), "SSL_enable_ct"},
    {ERR_FUNC(SSL_F_SSL_GET_NEW_SESSION), "ssl_get_new_session"},
    {ERR_FUNC(SSL_F_SSL_GET_PREV_SESSION), "ssl_get_prev_session"},
    {ERR_FUNC(SSL_F_SSL_GET_SERVER_CERT_INDEX), "ssl_get_server_cert_index"},
    {ERR_FUNC(SSL_F_SSL_GET_SIGN_PKEY), "ssl_get_sign_pkey"},
    {ERR_FUNC(SSL_F_SSL_INIT_WBIO_BUFFER), "ssl_init_wbio_buffer"},
    {ERR_FUNC(SSL_F_SSL_LOAD_CLIENT_CA_FILE), "SSL_load_client_CA_file"},
    {ERR_FUNC(SSL_F_SSL_MODULE_INIT), "ssl_module_init"},
    {ERR_FUNC(SSL_F_SSL_NEW), "SSL_new"},
    {ERR_FUNC(SSL_F_SSL_PARSE_CLIENTHELLO_RENEGOTIATE_EXT),
     "ssl_parse_clienthello_renegotiate_ext"},
    {ERR_FUNC(SSL_F_SSL_PARSE_CLIENTHELLO_TLSEXT),
     "ssl_parse_clienthello_tlsext"},
    {ERR_FUNC(SSL_F_SSL_PARSE_CLIENTHELLO_USE_SRTP_EXT),
     "ssl_parse_clienthello_use_srtp_ext"},
    {ERR_FUNC(SSL_F_SSL_PARSE_SERVERHELLO_RENEGOTIATE_EXT),
     "ssl_parse_serverhello_renegotiate_ext"},
    {ERR_FUNC(SSL_F_SSL_PARSE_SERVERHELLO_TLSEXT),
     "ssl_parse_serverhello_tlsext"},
    {ERR_FUNC(SSL_F_SSL_PARSE_SERVERHELLO_USE_SRTP_EXT),
     "ssl_parse_serverhello_use_srtp_ext"},
    {ERR_FUNC(SSL_F_SSL_PEEK), "SSL_peek"},
    {ERR_FUNC(SSL_F_SSL_READ), "SSL_read"},
    {ERR_FUNC(SSL_F_SSL_SCAN_CLIENTHELLO_TLSEXT),
     "ssl_scan_clienthello_tlsext"},
    {ERR_FUNC(SSL_F_SSL_SCAN_SERVERHELLO_TLSEXT),
     "ssl_scan_serverhello_tlsext"},
    {ERR_FUNC(SSL_F_SSL_SESSION_DUP), "ssl_session_dup"},
    {ERR_FUNC(SSL_F_SSL_SESSION_NEW), "SSL_SESSION_new"},
    {ERR_FUNC(SSL_F_SSL_SESSION_PRINT_FP), "SSL_SESSION_print_fp"},
    {ERR_FUNC(SSL_F_SSL_SESSION_SET1_ID), "SSL_SESSION_set1_id"},
    {ERR_FUNC(SSL_F_SSL_SESSION_SET1_ID_CONTEXT),
     "SSL_SESSION_set1_id_context"},
    {ERR_FUNC(SSL_F_SSL_SET_ALPN_PROTOS), "SSL_set_alpn_protos"},
    {ERR_FUNC(SSL_F_SSL_SET_CERT), "ssl_set_cert"},
    {ERR_FUNC(SSL_F_SSL_SET_CIPHER_LIST), "SSL_set_cipher_list"},
    {ERR_FUNC(SSL_F_SSL_SET_CT_VALIDATION_CALLBACK),
     "SSL_set_ct_validation_callback"},
    {ERR_FUNC(SSL_F_SSL_SET_FD), "SSL_set_fd"},
    {ERR_FUNC(SSL_F_SSL_SET_PKEY), "ssl_set_pkey"},
    {ERR_FUNC(SSL_F_SSL_SET_RFD), "SSL_set_rfd"},
    {ERR_FUNC(SSL_F_SSL_SET_SESSION), "SSL_set_session"},
    {ERR_FUNC(SSL_F_SSL_SET_SESSION_ID_CONTEXT),
     "SSL_set_session_id_context"},
    {ERR_FUNC(SSL_F_SSL_SET_SESSION_TICKET_EXT),
     "SSL_set_session_ticket_ext"},
    {ERR_FUNC(SSL_F_SSL_SET_WFD), "SSL_set_wfd"},
    {ERR_FUNC(SSL_F_SSL_SHUTDOWN), "SSL_shutdown"},
    {ERR_FUNC(SSL_F_SSL_SRP_CTX_INIT), "SSL_SRP_CTX_init"},
    {ERR_FUNC(SSL_F_SSL_START_ASYNC_JOB), "ssl_start_async_job"},
    {ERR_FUNC(SSL_F_SSL_UNDEFINED_FUNCTION), "ssl_undefined_function"},
    {ERR_FUNC(SSL_F_SSL_UNDEFINED_VOID_FUNCTION),
     "ssl_undefined_void_function"},
    {ERR_FUNC(SSL_F_SSL_USE_CERTIFICATE), "SSL_use_certificate"},
    {ERR_FUNC(SSL_F_SSL_USE_CERTIFICATE_ASN1), "SSL_use_certificate_ASN1"},
    {ERR_FUNC(SSL_F_SSL_USE_CERTIFICATE_FILE), "SSL_use_certificate_file"},
    {ERR_FUNC(SSL_F_SSL_USE_PRIVATEKEY), "SSL_use_PrivateKey"},
    {ERR_FUNC(SSL_F_SSL_USE_PRIVATEKEY_ASN1), "SSL_use_PrivateKey_ASN1"},
    {ERR_FUNC(SSL_F_SSL_USE_PRIVATEKEY_FILE), "SSL_use_PrivateKey_file"},
    {ERR_FUNC(SSL_F_SSL_USE_PSK_IDENTITY_HINT), "SSL_use_psk_identity_hint"},
    {ERR_FUNC(SSL_F_SSL_USE_RSAPRIVATEKEY), "SSL_use_RSAPrivateKey"},
    {ERR_FUNC(SSL_F_SSL_USE_RSAPRIVATEKEY_ASN1),
     "SSL_use_RSAPrivateKey_ASN1"},
    {ERR_FUNC(SSL_F_SSL_USE_RSAPRIVATEKEY_FILE),
     "SSL_use_RSAPrivateKey_file"},
    {ERR_FUNC(SSL_F_SSL_VALIDATE_CT), "ssl_validate_ct"},
    {ERR_FUNC(SSL_F_SSL_VERIFY_CERT_CHAIN), "ssl_verify_cert_chain"},
    {ERR_FUNC(SSL_F_SSL_WRITE), "SSL_write"},
    {ERR_FUNC(SSL_F_STATE_MACHINE), "state_machine"},
    {ERR_FUNC(SSL_F_TLS12_CHECK_PEER_SIGALG), "tls12_check_peer_sigalg"},
    {ERR_FUNC(SSL_F_TLS1_CHANGE_CIPHER_STATE), "tls1_change_cipher_state"},
    {ERR_FUNC(SSL_F_TLS1_CHECK_DUPLICATE_EXTENSIONS),
     "tls1_check_duplicate_extensions"},
    {ERR_FUNC(SSL_F_TLS1_ENC), "tls1_enc"},
    {ERR_FUNC(SSL_F_TLS1_EXPORT_KEYING_MATERIAL),
     "tls1_export_keying_material"},
    {ERR_FUNC(SSL_F_TLS1_GET_CURVELIST), "tls1_get_curvelist"},
    {ERR_FUNC(SSL_F_TLS1_PRF), "tls1_PRF"},
    {ERR_FUNC(SSL_F_TLS1_SETUP_KEY_BLOCK), "tls1_setup_key_block"},
    {ERR_FUNC(SSL_F_TLS1_SET_SERVER_SIGALGS), "tls1_set_server_sigalgs"},
    {ERR_FUNC(SSL_F_TLS_CLIENT_KEY_EXCHANGE_POST_WORK),
     "tls_client_key_exchange_post_work"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CERTIFICATE_REQUEST),
     "tls_construct_certificate_request"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CKE_DHE), "tls_construct_cke_dhe"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CKE_ECDHE), "tls_construct_cke_ecdhe"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CKE_GOST), "tls_construct_cke_gost"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CKE_PSK_PREAMBLE),
     "tls_construct_cke_psk_preamble"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CKE_RSA), "tls_construct_cke_rsa"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CKE_SRP), "tls_construct_cke_srp"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CLIENT_CERTIFICATE),
     "tls_construct_client_certificate"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CLIENT_HELLO),
     "tls_construct_client_hello"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CLIENT_KEY_EXCHANGE),
     "tls_construct_client_key_exchange"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_CLIENT_VERIFY),
     "tls_construct_client_verify"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_FINISHED), "tls_construct_finished"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_HELLO_REQUEST),
     "tls_construct_hello_request"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_NEW_SESSION_TICKET),
     "tls_construct_new_session_ticket"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_SERVER_CERTIFICATE),
     "tls_construct_server_certificate"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_SERVER_DONE), "tls_construct_server_done"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_SERVER_HELLO),
     "tls_construct_server_hello"},
    {ERR_FUNC(SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE),
     "tls_construct_server_key_exchange"},
    {ERR_FUNC(SSL_F_TLS_GET_MESSAGE_BODY), "tls_get_message_body"},
    {ERR_FUNC(SSL_F_TLS_GET_MESSAGE_HEADER), "tls_get_message_header"},
    {ERR_FUNC(SSL_F_TLS_POST_PROCESS_CLIENT_HELLO),
     "tls_post_process_client_hello"},
    {ERR_FUNC(SSL_F_TLS_POST_PROCESS_CLIENT_KEY_EXCHANGE),
     "tls_post_process_client_key_exchange"},
    {ERR_FUNC(SSL_F_TLS_PREPARE_CLIENT_CERTIFICATE),
     "tls_prepare_client_certificate"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CERTIFICATE_REQUEST),
     "tls_process_certificate_request"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CERT_STATUS), "tls_process_cert_status"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CERT_VERIFY), "tls_process_cert_verify"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CHANGE_CIPHER_SPEC),
     "tls_process_change_cipher_spec"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CKE_DHE), "tls_process_cke_dhe"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CKE_ECDHE), "tls_process_cke_ecdhe"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CKE_GOST), "tls_process_cke_gost"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CKE_PSK_PREAMBLE),
     "tls_process_cke_psk_preamble"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CKE_RSA), "tls_process_cke_rsa"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CKE_SRP), "tls_process_cke_srp"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE),
     "tls_process_client_certificate"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CLIENT_HELLO), "tls_process_client_hello"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_CLIENT_KEY_EXCHANGE),
     "tls_process_client_key_exchange"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_FINISHED), "tls_process_finished"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_KEY_EXCHANGE), "tls_process_key_exchange"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_NEW_SESSION_TICKET),
     "tls_process_new_session_ticket"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_NEXT_PROTO), "tls_process_next_proto"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_SERVER_CERTIFICATE),
     "tls_process_server_certificate"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_SERVER_DONE), "tls_process_server_done"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_SERVER_HELLO), "tls_process_server_hello"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_SKE_DHE), "tls_process_ske_dhe"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_SKE_ECDHE), "tls_process_ske_ecdhe"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_SKE_PSK_PREAMBLE),
     "tls_process_ske_psk_preamble"},
    {ERR_FUNC(SSL_F_TLS_PROCESS_SKE_SRP), "tls_process_ske_srp"},
    {ERR_FUNC(SSL_F_USE_CERTIFICATE_CHAIN_FILE),
     "use_certificate_chain_file"},
    {0, NULL}
};

static ERR_STRING_DATA SSL_str_reasons[] = {
    {ERR_REASON(SSL_R_APP_DATA_IN_HANDSHAKE), "app data in handshake"},
    {ERR_REASON(SSL_R_ATTEMPT_TO_REUSE_SESSION_IN_DIFFERENT_CONTEXT),
     "attempt to reuse session in different context"},
    {ERR_REASON(SSL_R_AT_LEAST_TLS_1_0_NEEDED_IN_FIPS_MODE),
     "at least TLS 1.0 needed in FIPS mode"},
    {ERR_REASON(SSL_R_AT_LEAST_TLS_1_2_NEEDED_IN_SUITEB_MODE),
     "at least (D)TLS 1.2 needed in Suite B mode"},
    {ERR_REASON(SSL_R_BAD_CHANGE_CIPHER_SPEC), "bad change cipher spec"},
    {ERR_REASON(SSL_R_BAD_DATA), "bad data"},
    {ERR_REASON(SSL_R_BAD_DATA_RETURNED_BY_CALLBACK),
     "bad data returned by callback"},
    {ERR_REASON(SSL_R_BAD_DECOMPRESSION), "bad decompression"},
    {ERR_REASON(SSL_R_BAD_DH_VALUE), "bad dh value"},
    {ERR_REASON(SSL_R_BAD_DIGEST_LENGTH), "bad digest length"},
    {ERR_REASON(SSL_R_BAD_ECC_CERT), "bad ecc cert"},
    {ERR_REASON(SSL_R_BAD_ECPOINT), "bad ecpoint"},
    {ERR_REASON(SSL_R_BAD_HANDSHAKE_LENGTH), "bad handshake length"},
    {ERR_REASON(SSL_R_BAD_HELLO_REQUEST), "bad hello request"},
    {ERR_REASON(SSL_R_BAD_LENGTH), "bad length"},
    {ERR_REASON(SSL_R_BAD_PACKET_LENGTH), "bad packet length"},
    {ERR_REASON(SSL_R_BAD_PROTOCOL_VERSION_NUMBER),
     "bad protocol version number"},
    {ERR_REASON(SSL_R_BAD_RSA_ENCRYPT), "bad rsa encrypt"},
    {ERR_REASON(SSL_R_BAD_SIGNATURE), "bad signature"},
    {ERR_REASON(SSL_R_BAD_SRP_A_LENGTH), "bad srp a length"},
    {ERR_REASON(SSL_R_BAD_SRP_PARAMETERS), "bad srp parameters"},
    {ERR_REASON(SSL_R_BAD_SRTP_MKI_VALUE), "bad srtp mki value"},
    {ERR_REASON(SSL_R_BAD_SRTP_PROTECTION_PROFILE_LIST),
     "bad srtp protection profile list"},
    {ERR_REASON(SSL_R_BAD_SSL_FILETYPE), "bad ssl filetype"},
    {ERR_REASON(SSL_R_BAD_VALUE), "bad value"},
    {ERR_REASON(SSL_R_BAD_WRITE_RETRY), "bad write retry"},
    {ERR_REASON(SSL_R_BIO_NOT_SET), "bio not set"},
    {ERR_REASON(SSL_R_BLOCK_CIPHER_PAD_IS_WRONG),
     "block cipher pad is wrong"},
    {ERR_REASON(SSL_R_BN_LIB), "bn lib"},
    {ERR_REASON(SSL_R_CA_DN_LENGTH_MISMATCH), "ca dn length mismatch"},
    {ERR_REASON(SSL_R_CA_KEY_TOO_SMALL), "ca key too small"},
    {ERR_REASON(SSL_R_CA_MD_TOO_WEAK), "ca md too weak"},
    {ERR_REASON(SSL_R_CCS_RECEIVED_EARLY), "ccs received early"},
    {ERR_REASON(SSL_R_CERTIFICATE_VERIFY_FAILED),
     "certificate verify failed"},
    {ERR_REASON(SSL_R_CERT_CB_ERROR), "cert cb error"},
    {ERR_REASON(SSL_R_CERT_LENGTH_MISMATCH), "cert length mismatch"},
    {ERR_REASON(SSL_R_CIPHER_CODE_WRONG_LENGTH), "cipher code wrong length"},
    {ERR_REASON(SSL_R_CIPHER_OR_HASH_UNAVAILABLE),
     "cipher or hash unavailable"},
    {ERR_REASON(SSL_R_CLIENTHELLO_TLSEXT), "clienthello tlsext"},
    {ERR_REASON(SSL_R_COMPRESSED_LENGTH_TOO_LONG),
     "compressed length too long"},
    {ERR_REASON(SSL_R_COMPRESSION_DISABLED), "compression disabled"},
    {ERR_REASON(SSL_R_COMPRESSION_FAILURE), "compression failure"},
    {ERR_REASON(SSL_R_COMPRESSION_ID_NOT_WITHIN_PRIVATE_RANGE),
     "compression id not within private range"},
    {ERR_REASON(SSL_R_COMPRESSION_LIBRARY_ERROR),
     "compression library error"},
    {ERR_REASON(SSL_R_CONNECTION_TYPE_NOT_SET), "connection type not set"},
    {ERR_REASON(SSL_R_CONTEXT_NOT_DANE_ENABLED), "context not dane enabled"},
    {ERR_REASON(SSL_R_COOKIE_GEN_CALLBACK_FAILURE),
     "cookie gen callback failure"},
    {ERR_REASON(SSL_R_COOKIE_MISMATCH), "cookie mismatch"},
    {ERR_REASON(SSL_R_CUSTOM_EXT_HANDLER_ALREADY_INSTALLED),
     "custom ext handler already installed"},
    {ERR_REASON(SSL_R_DANE_ALREADY_ENABLED), "dane already enabled"},
    {ERR_REASON(SSL_R_DANE_CANNOT_OVERRIDE_MTYPE_FULL),
     "dane cannot override mtype full"},
    {ERR_REASON(SSL_R_DANE_NOT_ENABLED), "dane not enabled"},
    {ERR_REASON(SSL_R_DANE_TLSA_BAD_CERTIFICATE),
     "dane tlsa bad certificate"},
    {ERR_REASON(SSL_R_DANE_TLSA_BAD_CERTIFICATE_USAGE),
     "dane tlsa bad certificate usage"},
    {ERR_REASON(SSL_R_DANE_TLSA_BAD_DATA_LENGTH),
     "dane tlsa bad data length"},
    {ERR_REASON(SSL_R_DANE_TLSA_BAD_DIGEST_LENGTH),
     "dane tlsa bad digest length"},
    {ERR_REASON(SSL_R_DANE_TLSA_BAD_MATCHING_TYPE),
     "dane tlsa bad matching type"},
    {ERR_REASON(SSL_R_DANE_TLSA_BAD_PUBLIC_KEY), "dane tlsa bad public key"},
    {ERR_REASON(SSL_R_DANE_TLSA_BAD_SELECTOR), "dane tlsa bad selector"},
    {ERR_REASON(SSL_R_DANE_TLSA_NULL_DATA), "dane tlsa null data"},
    {ERR_REASON(SSL_R_DATA_BETWEEN_CCS_AND_FINISHED),
     "data between ccs and finished"},
    {ERR_REASON(SSL_R_DATA_LENGTH_TOO_LONG), "data length too long"},
    {ERR_REASON(SSL_R_DECRYPTION_FAILED), "decryption failed"},
    {ERR_REASON(SSL_R_DECRYPTION_FAILED_OR_BAD_RECORD_MAC),
     "decryption failed or bad record mac"},
    {ERR_REASON(SSL_R_DH_KEY_TOO_SMALL), "dh key too small"},
    {ERR_REASON(SSL_R_DH_PUBLIC_VALUE_LENGTH_IS_WRONG),
     "dh public value length is wrong"},
    {ERR_REASON(SSL_R_DIGEST_CHECK_FAILED), "digest check failed"},
    {ERR_REASON(SSL_R_DTLS_MESSAGE_TOO_BIG), "dtls message too big"},
    {ERR_REASON(SSL_R_DUPLICATE_COMPRESSION_ID), "duplicate compression id"},
    {ERR_REASON(SSL_R_ECC_CERT_NOT_FOR_SIGNING), "ecc cert not for signing"},
    {ERR_REASON(SSL_R_ECDH_REQUIRED_FOR_SUITEB_MODE),
     "ecdh required for suiteb mode"},
    {ERR_REASON(SSL_R_EE_KEY_TOO_SMALL), "ee key too small"},
    {ERR_REASON(SSL_R_EMPTY_SRTP_PROTECTION_PROFILE_LIST),
     "empty srtp protection profile list"},
    {ERR_REASON(SSL_R_ENCRYPTED_LENGTH_TOO_LONG),
     "encrypted length too long"},
    {ERR_REASON(SSL_R_ERROR_IN_RECEIVED_CIPHER_LIST),
     "error in received cipher list"},
    {ERR_REASON(SSL_R_ERROR_SETTING_TLSA_BASE_DOMAIN),
     "error setting tlsa base domain"},
    {ERR_REASON(SSL_R_EXCESSIVE_MESSAGE_SIZE), "excessive message size"},
    {ERR_REASON(SSL_R_EXTRA_DATA_IN_MESSAGE), "extra data in message"},
    {ERR_REASON(SSL_R_FAILED_TO_INIT_ASYNC), "failed to init async"},
    {ERR_REASON(SSL_R_FRAGMENTED_CLIENT_HELLO), "fragmented client hello"},
    {ERR_REASON(SSL_R_GOT_A_FIN_BEFORE_A_CCS), "got a fin before a ccs"},
    {ERR_REASON(SSL_R_HTTPS_PROXY_REQUEST), "https proxy request"},
    {ERR_REASON(SSL_R_HTTP_REQUEST), "http request"},
    {ERR_REASON(SSL_R_ILLEGAL_SUITEB_DIGEST), "illegal Suite B digest"},
    {ERR_REASON(SSL_R_INAPPROPRIATE_FALLBACK), "inappropriate fallback"},
    {ERR_REASON(SSL_R_INCONSISTENT_COMPRESSION), "inconsistent compression"},
    {ERR_REASON(SSL_R_INCONSISTENT_EXTMS), "inconsistent extms"},
    {ERR_REASON(SSL_R_INVALID_COMMAND), "invalid command"},
    {ERR_REASON(SSL_R_INVALID_COMPRESSION_ALGORITHM),
     "invalid compression algorithm"},
    {ERR_REASON(SSL_R_INVALID_CONFIGURATION_NAME),
     "invalid configuration name"},
    {ERR_REASON(SSL_R_INVALID_CT_VALIDATION_TYPE),
     "invalid ct validation type"},
    {ERR_REASON(SSL_R_INVALID_NULL_CMD_NAME), "invalid null cmd name"},
    {ERR_REASON(SSL_R_INVALID_SEQUENCE_NUMBER), "invalid sequence number"},
    {ERR_REASON(SSL_R_INVALID_SERVERINFO_DATA), "invalid serverinfo data"},
    {ERR_REASON(SSL_R_INVALID_SRP_USERNAME), "invalid srp username"},
    {ERR_REASON(SSL_R_INVALID_STATUS_RESPONSE), "invalid status response"},
    {ERR_REASON(SSL_R_INVALID_TICKET_KEYS_LENGTH),
     "invalid ticket keys length"},
    {ERR_REASON(SSL_R_LENGTH_MISMATCH), "length mismatch"},
    {ERR_REASON(SSL_R_LENGTH_TOO_LONG), "length too long"},
    {ERR_REASON(SSL_R_LENGTH_TOO_SHORT), "length too short"},
    {ERR_REASON(SSL_R_LIBRARY_BUG), "library bug"},
    {ERR_REASON(SSL_R_LIBRARY_HAS_NO_CIPHERS), "library has no ciphers"},
    {ERR_REASON(SSL_R_MISSING_DSA_SIGNING_CERT), "missing dsa signing cert"},
    {ERR_REASON(SSL_R_MISSING_ECDSA_SIGNING_CERT),
     "missing ecdsa signing cert"},
    {ERR_REASON(SSL_R_MISSING_RSA_CERTIFICATE), "missing rsa certificate"},
    {ERR_REASON(SSL_R_MISSING_RSA_ENCRYPTING_CERT),
     "missing rsa encrypting cert"},
    {ERR_REASON(SSL_R_MISSING_RSA_SIGNING_CERT), "missing rsa signing cert"},
    {ERR_REASON(SSL_R_MISSING_SRP_PARAM), "can't find SRP server param"},
    {ERR_REASON(SSL_R_MISSING_TMP_DH_KEY), "missing tmp dh key"},
    {ERR_REASON(SSL_R_MISSING_TMP_ECDH_KEY), "missing tmp ecdh key"},
    {ERR_REASON(SSL_R_NO_CERTIFICATES_RETURNED), "no certificates returned"},
    {ERR_REASON(SSL_R_NO_CERTIFICATE_ASSIGNED), "no certificate assigned"},
    {ERR_REASON(SSL_R_NO_CERTIFICATE_SET), "no certificate set"},
    {ERR_REASON(SSL_R_NO_CIPHERS_AVAILABLE), "no ciphers available"},
    {ERR_REASON(SSL_R_NO_CIPHERS_SPECIFIED), "no ciphers specified"},
    {ERR_REASON(SSL_R_NO_CIPHER_MATCH), "no cipher match"},
    {ERR_REASON(SSL_R_NO_CLIENT_CERT_METHOD), "no client cert method"},
    {ERR_REASON(SSL_R_NO_COMPRESSION_SPECIFIED), "no compression specified"},
    {ERR_REASON(SSL_R_NO_GOST_CERTIFICATE_SENT_BY_PEER),
     "Peer haven't sent GOST certificate, required for selected ciphersuite"},
    {ERR_REASON(SSL_R_NO_METHOD_SPECIFIED), "no method specified"},
    {ERR_REASON(SSL_R_NO_PEM_EXTENSIONS), "no pem extensions"},
    {ERR_REASON(SSL_R_NO_PRIVATE_KEY_ASSIGNED), "no private key assigned"},
    {ERR_REASON(SSL_R_NO_PROTOCOLS_AVAILABLE), "no protocols available"},
    {ERR_REASON(SSL_R_NO_RENEGOTIATION), "no renegotiation"},
    {ERR_REASON(SSL_R_NO_REQUIRED_DIGEST), "no required digest"},
    {ERR_REASON(SSL_R_NO_SHARED_CIPHER), "no shared cipher"},
    {ERR_REASON(SSL_R_NO_SHARED_SIGNATURE_ALGORITHMS),
     "no shared signature algorithms"},
    {ERR_REASON(SSL_R_NO_SRTP_PROFILES), "no srtp profiles"},
    {ERR_REASON(SSL_R_NO_VALID_SCTS), "no valid scts"},
    {ERR_REASON(SSL_R_NO_VERIFY_COOKIE_CALLBACK),
     "no verify cookie callback"},
    {ERR_REASON(SSL_R_NULL_SSL_CTX), "null ssl ctx"},
    {ERR_REASON(SSL_R_NULL_SSL_METHOD_PASSED), "null ssl method passed"},
    {ERR_REASON(SSL_R_OLD_SESSION_CIPHER_NOT_RETURNED),
     "old session cipher not returned"},
    {ERR_REASON(SSL_R_OLD_SESSION_COMPRESSION_ALGORITHM_NOT_RETURNED),
     "old session compression algorithm not returned"},
    {ERR_REASON(SSL_R_PACKET_LENGTH_TOO_LONG), "packet length too long"},
    {ERR_REASON(SSL_R_PARSE_TLSEXT), "parse tlsext"},
    {ERR_REASON(SSL_R_PATH_TOO_LONG), "path too long"},
    {ERR_REASON(SSL_R_PEER_DID_NOT_RETURN_A_CERTIFICATE),
     "peer did not return a certificate"},
    {ERR_REASON(SSL_R_PEM_NAME_BAD_PREFIX), "pem name bad prefix"},
    {ERR_REASON(SSL_R_PEM_NAME_TOO_SHORT), "pem name too short"},
    {ERR_REASON(SSL_R_PIPELINE_FAILURE), "pipeline failure"},
    {ERR_REASON(SSL_R_PROTOCOL_IS_SHUTDOWN), "protocol is shutdown"},
    {ERR_REASON(SSL_R_PSK_IDENTITY_NOT_FOUND), "psk identity not found"},
    {ERR_REASON(SSL_R_PSK_NO_CLIENT_CB), "psk no client cb"},
    {ERR_REASON(SSL_R_PSK_NO_SERVER_CB), "psk no server cb"},
    {ERR_REASON(SSL_R_READ_BIO_NOT_SET), "read bio not set"},
    {ERR_REASON(SSL_R_READ_TIMEOUT_EXPIRED), "read timeout expired"},
    {ERR_REASON(SSL_R_RECORD_LENGTH_MISMATCH), "record length mismatch"},
    {ERR_REASON(SSL_R_RECORD_TOO_SMALL), "record too small"},
    {ERR_REASON(SSL_R_RENEGOTIATE_EXT_TOO_LONG), "renegotiate ext too long"},
    {ERR_REASON(SSL_R_RENEGOTIATION_ENCODING_ERR),
     "renegotiation encoding err"},
    {ERR_REASON(SSL_R_RENEGOTIATION_MISMATCH), "renegotiation mismatch"},
    {ERR_REASON(SSL_R_REQUIRED_CIPHER_MISSING), "required cipher missing"},
    {ERR_REASON(SSL_R_REQUIRED_COMPRESSION_ALGORITHM_MISSING),
     "required compression algorithm missing"},
    {ERR_REASON(SSL_R_SCSV_RECEIVED_WHEN_RENEGOTIATING),
     "scsv received when renegotiating"},
    {ERR_REASON(SSL_R_SCT_VERIFICATION_FAILED), "sct verification failed"},
    {ERR_REASON(SSL_R_SERVERHELLO_TLSEXT), "serverhello tlsext"},
    {ERR_REASON(SSL_R_SESSION_ID_CONTEXT_UNINITIALIZED),
     "session id context uninitialized"},
    {ERR_REASON(SSL_R_SHUTDOWN_WHILE_IN_INIT), "shutdown while in init"},
    {ERR_REASON(SSL_R_SIGNATURE_ALGORITHMS_ERROR),
     "signature algorithms error"},
    {ERR_REASON(SSL_R_SIGNATURE_FOR_NON_SIGNING_CERTIFICATE),
     "signature for non signing certificate"},
    {ERR_REASON(SSL_R_SRP_A_CALC), "error with the srp params"},
    {ERR_REASON(SSL_R_SRTP_COULD_NOT_ALLOCATE_PROFILES),
     "srtp could not allocate profiles"},
    {ERR_REASON(SSL_R_SRTP_PROTECTION_PROFILE_LIST_TOO_LONG),
     "srtp protection profile list too long"},
    {ERR_REASON(SSL_R_SRTP_UNKNOWN_PROTECTION_PROFILE),
     "srtp unknown protection profile"},
    {ERR_REASON(SSL_R_SSL3_EXT_INVALID_SERVERNAME),
     "ssl3 ext invalid servername"},
    {ERR_REASON(SSL_R_SSL3_EXT_INVALID_SERVERNAME_TYPE),
     "ssl3 ext invalid servername type"},
    {ERR_REASON(SSL_R_SSL3_SESSION_ID_TOO_LONG), "ssl3 session id too long"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_BAD_CERTIFICATE),
     "sslv3 alert bad certificate"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_BAD_RECORD_MAC),
     "sslv3 alert bad record mac"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_CERTIFICATE_EXPIRED),
     "sslv3 alert certificate expired"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_CERTIFICATE_REVOKED),
     "sslv3 alert certificate revoked"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_CERTIFICATE_UNKNOWN),
     "sslv3 alert certificate unknown"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_DECOMPRESSION_FAILURE),
     "sslv3 alert decompression failure"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_HANDSHAKE_FAILURE),
     "sslv3 alert handshake failure"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_ILLEGAL_PARAMETER),
     "sslv3 alert illegal parameter"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_NO_CERTIFICATE),
     "sslv3 alert no certificate"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_UNEXPECTED_MESSAGE),
     "sslv3 alert unexpected message"},
    {ERR_REASON(SSL_R_SSLV3_ALERT_UNSUPPORTED_CERTIFICATE),
     "sslv3 alert unsupported certificate"},
    {ERR_REASON(SSL_R_SSL_COMMAND_SECTION_EMPTY),
     "ssl command section empty"},
    {ERR_REASON(SSL_R_SSL_COMMAND_SECTION_NOT_FOUND),
     "ssl command section not found"},
    {ERR_REASON(SSL_R_SSL_CTX_HAS_NO_DEFAULT_SSL_VERSION),
     "ssl ctx has no default ssl version"},
    {ERR_REASON(SSL_R_SSL_HANDSHAKE_FAILURE), "ssl handshake failure"},
    {ERR_REASON(SSL_R_SSL_LIBRARY_HAS_NO_CIPHERS),
     "ssl library has no ciphers"},
    {ERR_REASON(SSL_R_SSL_NEGATIVE_LENGTH), "ssl negative length"},
    {ERR_REASON(SSL_R_SSL_SECTION_EMPTY), "ssl section empty"},
    {ERR_REASON(SSL_R_SSL_SECTION_NOT_FOUND), "ssl section not found"},
    {ERR_REASON(SSL_R_SSL_SESSION_ID_CALLBACK_FAILED),
     "ssl session id callback failed"},
    {ERR_REASON(SSL_R_SSL_SESSION_ID_CONFLICT), "ssl session id conflict"},
    {ERR_REASON(SSL_R_SSL_SESSION_ID_CONTEXT_TOO_LONG),
     "ssl session id context too long"},
    {ERR_REASON(SSL_R_SSL_SESSION_ID_TOO_LONG),
     "ssl session id too long"},
    {ERR_REASON(SSL_R_SSL_SESSION_ID_HAS_BAD_LENGTH),
     "ssl session id has bad length"},
    {ERR_REASON(SSL_R_SSL_SESSION_VERSION_MISMATCH),
     "ssl session version mismatch"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_ACCESS_DENIED),
     "tlsv1 alert access denied"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_DECODE_ERROR), "tlsv1 alert decode error"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_DECRYPTION_FAILED),
     "tlsv1 alert decryption failed"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_DECRYPT_ERROR),
     "tlsv1 alert decrypt error"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_EXPORT_RESTRICTION),
     "tlsv1 alert export restriction"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_INAPPROPRIATE_FALLBACK),
     "tlsv1 alert inappropriate fallback"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_INSUFFICIENT_SECURITY),
     "tlsv1 alert insufficient security"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_INTERNAL_ERROR),
     "tlsv1 alert internal error"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_NO_RENEGOTIATION),
     "tlsv1 alert no renegotiation"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_PROTOCOL_VERSION),
     "tlsv1 alert protocol version"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_RECORD_OVERFLOW),
     "tlsv1 alert record overflow"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_UNKNOWN_CA), "tlsv1 alert unknown ca"},
    {ERR_REASON(SSL_R_TLSV1_ALERT_USER_CANCELLED),
     "tlsv1 alert user cancelled"},
    {ERR_REASON(SSL_R_TLSV1_BAD_CERTIFICATE_HASH_VALUE),
     "tlsv1 bad certificate hash value"},
    {ERR_REASON(SSL_R_TLSV1_BAD_CERTIFICATE_STATUS_RESPONSE),
     "tlsv1 bad certificate status response"},
    {ERR_REASON(SSL_R_TLSV1_CERTIFICATE_UNOBTAINABLE),
     "tlsv1 certificate unobtainable"},
    {ERR_REASON(SSL_R_TLSV1_UNRECOGNIZED_NAME), "tlsv1 unrecognized name"},
    {ERR_REASON(SSL_R_TLSV1_UNSUPPORTED_EXTENSION),
     "tlsv1 unsupported extension"},
    {ERR_REASON(SSL_R_TLS_HEARTBEAT_PEER_DOESNT_ACCEPT),
     "peer does not accept heartbeats"},
    {ERR_REASON(SSL_R_TLS_HEARTBEAT_PENDING),
     "heartbeat request already pending"},
    {ERR_REASON(SSL_R_TLS_ILLEGAL_EXPORTER_LABEL),
     "tls illegal exporter label"},
    {ERR_REASON(SSL_R_TLS_INVALID_ECPOINTFORMAT_LIST),
     "tls invalid ecpointformat list"},
    {ERR_REASON(SSL_R_TOO_MANY_WARN_ALERTS), "too many warn alerts"},
    {ERR_REASON(SSL_R_UNABLE_TO_FIND_ECDH_PARAMETERS),
     "unable to find ecdh parameters"},
    {ERR_REASON(SSL_R_UNABLE_TO_FIND_PUBLIC_KEY_PARAMETERS),
     "unable to find public key parameters"},
    {ERR_REASON(SSL_R_UNABLE_TO_LOAD_SSL3_MD5_ROUTINES),
     "unable to load ssl3 md5 routines"},
    {ERR_REASON(SSL_R_UNABLE_TO_LOAD_SSL3_SHA1_ROUTINES),
     "unable to load ssl3 sha1 routines"},
    {ERR_REASON(SSL_R_UNEXPECTED_MESSAGE), "unexpected message"},
    {ERR_REASON(SSL_R_UNEXPECTED_RECORD), "unexpected record"},
    {ERR_REASON(SSL_R_UNINITIALIZED), "uninitialized"},
    {ERR_REASON(SSL_R_UNKNOWN_ALERT_TYPE), "unknown alert type"},
    {ERR_REASON(SSL_R_UNKNOWN_CERTIFICATE_TYPE), "unknown certificate type"},
    {ERR_REASON(SSL_R_UNKNOWN_CIPHER_RETURNED), "unknown cipher returned"},
    {ERR_REASON(SSL_R_UNKNOWN_CIPHER_TYPE), "unknown cipher type"},
    {ERR_REASON(SSL_R_UNKNOWN_CMD_NAME), "unknown cmd name"},
    {ERR_REASON(SSL_R_UNKNOWN_COMMAND), "unknown command"},
    {ERR_REASON(SSL_R_UNKNOWN_DIGEST), "unknown digest"},
    {ERR_REASON(SSL_R_UNKNOWN_KEY_EXCHANGE_TYPE),
     "unknown key exchange type"},
    {ERR_REASON(SSL_R_UNKNOWN_PKEY_TYPE), "unknown pkey type"},
    {ERR_REASON(SSL_R_UNKNOWN_PROTOCOL), "unknown protocol"},
    {ERR_REASON(SSL_R_UNKNOWN_SSL_VERSION), "unknown ssl version"},
    {ERR_REASON(SSL_R_UNKNOWN_STATE), "unknown state"},
    {ERR_REASON(SSL_R_UNSAFE_LEGACY_RENEGOTIATION_DISABLED),
     "unsafe legacy renegotiation disabled"},
    {ERR_REASON(SSL_R_UNSUPPORTED_COMPRESSION_ALGORITHM),
     "unsupported compression algorithm"},
    {ERR_REASON(SSL_R_UNSUPPORTED_ELLIPTIC_CURVE),
     "unsupported elliptic curve"},
    {ERR_REASON(SSL_R_UNSUPPORTED_PROTOCOL), "unsupported protocol"},
    {ERR_REASON(SSL_R_UNSUPPORTED_SSL_VERSION), "unsupported ssl version"},
    {ERR_REASON(SSL_R_UNSUPPORTED_STATUS_TYPE), "unsupported status type"},
    {ERR_REASON(SSL_R_USE_SRTP_NOT_NEGOTIATED), "use srtp not negotiated"},
    {ERR_REASON(SSL_R_VERSION_TOO_HIGH), "version too high"},
    {ERR_REASON(SSL_R_VERSION_TOO_LOW), "version too low"},
    {ERR_REASON(SSL_R_WRONG_CERTIFICATE_TYPE), "wrong certificate type"},
    {ERR_REASON(SSL_R_WRONG_CIPHER_RETURNED), "wrong cipher returned"},
    {ERR_REASON(SSL_R_WRONG_CURVE), "wrong curve"},
    {ERR_REASON(SSL_R_WRONG_SIGNATURE_LENGTH), "wrong signature length"},
    {ERR_REASON(SSL_R_WRONG_SIGNATURE_SIZE), "wrong signature size"},
    {ERR_REASON(SSL_R_WRONG_SIGNATURE_TYPE), "wrong signature type"},
    {ERR_REASON(SSL_R_WRONG_SSL_VERSION), "wrong ssl version"},
    {ERR_REASON(SSL_R_WRONG_VERSION_NUMBER), "wrong version number"},
    {ERR_REASON(SSL_R_X509_LIB), "x509 lib"},
    {ERR_REASON(SSL_R_X509_VERIFICATION_SETUP_PROBLEMS),
     "x509 verification setup problems"},
    {0, NULL}
};

#endif

int ERR_load_SSL_strings(void)
{
#ifndef OPENSSL_NO_ERR

    if (ERR_func_error_string(SSL_str_functs[0].error) == NULL) {
        ERR_load_strings(0, SSL_str_functs);
        ERR_load_strings(0, SSL_str_reasons);
    }
#endif
    return 1;
}
