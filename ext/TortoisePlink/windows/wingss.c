#ifndef NO_GSSAPI

#include "putty.h"

#define SECURITY_WIN32
#include <security.h>

#include "pgssapi.h"
#include "sshgss.h"
#include "sshgssc.h"

#include "misc.h"

/* Windows code to set up the GSSAPI library list. */

const int ngsslibs = 3;
const char *const gsslibnames[3] = {
    "MIT Kerberos GSSAPI32.DLL",
    "Microsoft SSPI SECUR32.DLL",
    "User-specified GSSAPI DLL",
};
const struct keyvalwhere gsslibkeywords[] = {
    { "gssapi32", 0, -1, -1 },
    { "sspi", 1, -1, -1 },
    { "custom", 2, -1, -1 },
};

DECL_WINDOWS_FUNCTION(static, SECURITY_STATUS,
		      AcquireCredentialsHandleA,
		      (SEC_CHAR *, SEC_CHAR *, ULONG, PLUID,
		       PVOID, SEC_GET_KEY_FN, PVOID, PCredHandle, PTimeStamp));
DECL_WINDOWS_FUNCTION(static, SECURITY_STATUS,
		      InitializeSecurityContextA,
		      (PCredHandle, PCtxtHandle, SEC_CHAR *, ULONG, ULONG,
		       ULONG, PSecBufferDesc, ULONG, PCtxtHandle,
		       PSecBufferDesc, PULONG, PTimeStamp));
DECL_WINDOWS_FUNCTION(static, SECURITY_STATUS,
		      FreeContextBuffer,
		      (PVOID));
DECL_WINDOWS_FUNCTION(static, SECURITY_STATUS,
		      FreeCredentialsHandle,
		      (PCredHandle));
DECL_WINDOWS_FUNCTION(static, SECURITY_STATUS,
		      DeleteSecurityContext,
		      (PCtxtHandle));
DECL_WINDOWS_FUNCTION(static, SECURITY_STATUS,
		      QueryContextAttributesA,
		      (PCtxtHandle, ULONG, PVOID));
DECL_WINDOWS_FUNCTION(static, SECURITY_STATUS,
		      MakeSignature,
		      (PCtxtHandle, ULONG, PSecBufferDesc, ULONG));

typedef struct winSsh_gss_ctx {
    unsigned long maj_stat;
    unsigned long min_stat;
    CredHandle cred_handle;
    CtxtHandle context;
    PCtxtHandle context_handle;
    TimeStamp expiry;
} winSsh_gss_ctx;


const Ssh_gss_buf gss_mech_krb5={9,"\x2A\x86\x48\x86\xF7\x12\x01\x02\x02"};

const char *gsslogmsg = NULL;

static void ssh_sspi_bind_fns(struct ssh_gss_library *lib);

struct ssh_gss_liblist *ssh_gss_setup(Conf *conf)
{
    HMODULE module;
    HKEY regkey;
    struct ssh_gss_liblist *list = snew(struct ssh_gss_liblist);
    char *path;

    list->libraries = snewn(3, struct ssh_gss_library);
    list->nlibraries = 0;

    /* MIT Kerberos GSSAPI implementation */
    /* TODO: For 64-bit builds, check for gssapi64.dll */
    module = NULL;
    if (RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\MIT\\Kerberos", &regkey)
	== ERROR_SUCCESS) {
	DWORD type, size;
	LONG ret;
	char *buffer;

	/* Find out the string length */
        ret = RegQueryValueEx(regkey, "InstallDir", NULL, &type, NULL, &size);

	if (ret == ERROR_SUCCESS && type == REG_SZ) {
	    buffer = snewn(size + 20, char);
	    ret = RegQueryValueEx(regkey, "InstallDir", NULL,
				  &type, (LPBYTE)buffer, &size);
	    if (ret == ERROR_SUCCESS && type == REG_SZ) {
#ifdef _WIN64
		strcat(buffer, "\\bin\\gssapi64.dll");
#else
        strcat(buffer, "\\bin\\gssapi32.dll");
#endif
		module = LoadLibrary(buffer);
	    }
	    sfree(buffer);
	}
	RegCloseKey(regkey);
    }
    if (module) {
	struct ssh_gss_library *lib =
	    &list->libraries[list->nlibraries++];

	lib->id = 0;
	lib->gsslogmsg = "Using GSSAPI from GSSAPI32.DLL";
	lib->handle = (void *)module;

#define BIND_GSS_FN(name) \
    lib->u.gssapi.name = (t_gss_##name) GetProcAddress(module, "gss_" #name)

        BIND_GSS_FN(delete_sec_context);
        BIND_GSS_FN(display_status);
        BIND_GSS_FN(get_mic);
        BIND_GSS_FN(import_name);
        BIND_GSS_FN(init_sec_context);
        BIND_GSS_FN(release_buffer);
        BIND_GSS_FN(release_cred);
        BIND_GSS_FN(release_name);

#undef BIND_GSS_FN

        ssh_gssapi_bind_fns(lib);
    }

    /* Microsoft SSPI Implementation */
    module = load_system32_dll("secur32.dll");
    if (module) {
	struct ssh_gss_library *lib =
	    &list->libraries[list->nlibraries++];

	lib->id = 1;
	lib->gsslogmsg = "Using SSPI from SECUR32.DLL";
	lib->handle = (void *)module;

	GET_WINDOWS_FUNCTION(module, AcquireCredentialsHandleA);
	GET_WINDOWS_FUNCTION(module, InitializeSecurityContextA);
	GET_WINDOWS_FUNCTION(module, FreeContextBuffer);
	GET_WINDOWS_FUNCTION(module, FreeCredentialsHandle);
	GET_WINDOWS_FUNCTION(module, DeleteSecurityContext);
	GET_WINDOWS_FUNCTION(module, QueryContextAttributesA);
	GET_WINDOWS_FUNCTION(module, MakeSignature);

	ssh_sspi_bind_fns(lib);
    }

    /*
     * Custom GSSAPI DLL.
     */
    module = NULL;
    path = conf_get_filename(conf, CONF_ssh_gss_custom)->path;
    if (*path) {
	module = LoadLibrary(path);
    }
    if (module) {
	struct ssh_gss_library *lib =
	    &list->libraries[list->nlibraries++];

	lib->id = 2;
	lib->gsslogmsg = dupprintf("Using GSSAPI from user-specified"
				   " library '%s'", path);
	lib->handle = (void *)module;

#define BIND_GSS_FN(name) \
    lib->u.gssapi.name = (t_gss_##name) GetProcAddress(module, "gss_" #name)

        BIND_GSS_FN(delete_sec_context);
        BIND_GSS_FN(display_status);
        BIND_GSS_FN(get_mic);
        BIND_GSS_FN(import_name);
        BIND_GSS_FN(init_sec_context);
        BIND_GSS_FN(release_buffer);
        BIND_GSS_FN(release_cred);
        BIND_GSS_FN(release_name);

#undef BIND_GSS_FN

        ssh_gssapi_bind_fns(lib);
    }


    return list;
}

void ssh_gss_cleanup(struct ssh_gss_liblist *list)
{
    int i;

    /*
     * LoadLibrary and FreeLibrary are defined to employ reference
     * counting in the case where the same library is repeatedly
     * loaded, so even in a multiple-sessions-per-process context
     * (not that we currently expect ever to have such a thing on
     * Windows) it's safe to naively FreeLibrary everything here
     * without worrying about destroying it under the feet of
     * another SSH instance still using it.
     */
    for (i = 0; i < list->nlibraries; i++) {
	FreeLibrary((HMODULE)list->libraries[i].handle);
	if (list->libraries[i].id == 2) {
	    /* The 'custom' id involves a dynamically allocated message.
	     * Note that we must cast away the 'const' to free it. */
	    sfree((char *)list->libraries[i].gsslogmsg);
	}
    }
    sfree(list->libraries);
    sfree(list);
}

static Ssh_gss_stat ssh_sspi_indicate_mech(struct ssh_gss_library *lib,
					   Ssh_gss_buf *mech)
{
    *mech = gss_mech_krb5;
    return SSH_GSS_OK;
}


static Ssh_gss_stat ssh_sspi_import_name(struct ssh_gss_library *lib,
					 char *host, Ssh_gss_name *srv_name)
{
    char *pStr;

    /* Check hostname */
    if (host == NULL) return SSH_GSS_FAILURE;
    
    /* copy it into form host/FQDN */
    pStr = dupcat("host/", host, NULL);

    *srv_name = (Ssh_gss_name) pStr;

    return SSH_GSS_OK;
}

static Ssh_gss_stat ssh_sspi_acquire_cred(struct ssh_gss_library *lib,
					  Ssh_gss_ctx *ctx)
{
    winSsh_gss_ctx *winctx = snew(winSsh_gss_ctx);
    memset(winctx, 0, sizeof(winSsh_gss_ctx));

    /* prepare our "wrapper" structure */
    winctx->maj_stat =  winctx->min_stat = SEC_E_OK;
    winctx->context_handle = NULL;

    /* Specifying no principal name here means use the credentials of
       the current logged-in user */

    winctx->maj_stat = p_AcquireCredentialsHandleA(NULL,
						   "Kerberos",
						   SECPKG_CRED_OUTBOUND,
						   NULL,
						   NULL,
						   NULL,
						   NULL,
						   &winctx->cred_handle,
						   &winctx->expiry);

    if (winctx->maj_stat != SEC_E_OK) return SSH_GSS_FAILURE;
    
    *ctx = (Ssh_gss_ctx) winctx;
    return SSH_GSS_OK;
}


static Ssh_gss_stat ssh_sspi_init_sec_context(struct ssh_gss_library *lib,
					      Ssh_gss_ctx *ctx,
					      Ssh_gss_name srv_name,
					      int to_deleg,
					      Ssh_gss_buf *recv_tok,
					      Ssh_gss_buf *send_tok)
{
    winSsh_gss_ctx *winctx = (winSsh_gss_ctx *) *ctx;
    SecBuffer wsend_tok = {send_tok->length,SECBUFFER_TOKEN,send_tok->value};
    SecBuffer wrecv_tok = {recv_tok->length,SECBUFFER_TOKEN,recv_tok->value};
    SecBufferDesc output_desc={SECBUFFER_VERSION,1,&wsend_tok};
    SecBufferDesc input_desc ={SECBUFFER_VERSION,1,&wrecv_tok};
    unsigned long flags=ISC_REQ_MUTUAL_AUTH|ISC_REQ_REPLAY_DETECT|
	ISC_REQ_CONFIDENTIALITY|ISC_REQ_ALLOCATE_MEMORY;
    unsigned long ret_flags=0;
    
    /* check if we have to delegate ... */
    if (to_deleg) flags |= ISC_REQ_DELEGATE;
    winctx->maj_stat = p_InitializeSecurityContextA(&winctx->cred_handle,
						    winctx->context_handle,
						    (char*) srv_name,
						    flags,
						    0,          /* reserved */
						    SECURITY_NATIVE_DREP,
						    &input_desc,
						    0,          /* reserved */
						    &winctx->context,
						    &output_desc,
						    &ret_flags,
						    &winctx->expiry);
  
    /* prepare for the next round */
    winctx->context_handle = &winctx->context;
    send_tok->value = wsend_tok.pvBuffer;
    send_tok->length = wsend_tok.cbBuffer;
  
    /* check & return our status */
    if (winctx->maj_stat==SEC_E_OK) return SSH_GSS_S_COMPLETE;
    if (winctx->maj_stat==SEC_I_CONTINUE_NEEDED) return SSH_GSS_S_CONTINUE_NEEDED;
    
    return SSH_GSS_FAILURE;
}

static Ssh_gss_stat ssh_sspi_free_tok(struct ssh_gss_library *lib,
				      Ssh_gss_buf *send_tok)
{
    /* check input */
    if (send_tok == NULL) return SSH_GSS_FAILURE;

    /* free Windows buffer */
    p_FreeContextBuffer(send_tok->value);
    SSH_GSS_CLEAR_BUF(send_tok);
    
    return SSH_GSS_OK;
}

static Ssh_gss_stat ssh_sspi_release_cred(struct ssh_gss_library *lib,
					  Ssh_gss_ctx *ctx)
{
    winSsh_gss_ctx *winctx= (winSsh_gss_ctx *) *ctx;

    /* check input */
    if (winctx == NULL) return SSH_GSS_FAILURE;

    /* free Windows data */
    p_FreeCredentialsHandle(&winctx->cred_handle);
    p_DeleteSecurityContext(&winctx->context);

    /* delete our "wrapper" structure */
    sfree(winctx);
    *ctx = (Ssh_gss_ctx) NULL;

    return SSH_GSS_OK;
}


static Ssh_gss_stat ssh_sspi_release_name(struct ssh_gss_library *lib,
					  Ssh_gss_name *srv_name)
{
    char *pStr= (char *) *srv_name;

    if (pStr == NULL) return SSH_GSS_FAILURE;
    sfree(pStr);
    *srv_name = (Ssh_gss_name) NULL;

    return SSH_GSS_OK;
}

static Ssh_gss_stat ssh_sspi_display_status(struct ssh_gss_library *lib,
					    Ssh_gss_ctx ctx, Ssh_gss_buf *buf)
{
    winSsh_gss_ctx *winctx = (winSsh_gss_ctx *) ctx;
    const char *msg;

    if (winctx == NULL) return SSH_GSS_FAILURE;

    /* decode the error code */
    switch (winctx->maj_stat) {
      case SEC_E_OK: msg="SSPI status OK"; break;
      case SEC_E_INVALID_HANDLE: msg="The handle passed to the function"
	    " is invalid.";
	break;
      case SEC_E_TARGET_UNKNOWN: msg="The target was not recognized."; break;
      case SEC_E_LOGON_DENIED: msg="The logon failed."; break;
      case SEC_E_INTERNAL_ERROR: msg="The Local Security Authority cannot"
	    " be contacted.";
	break;
      case SEC_E_NO_CREDENTIALS: msg="No credentials are available in the"
	    " security package.";
	break;
      case SEC_E_NO_AUTHENTICATING_AUTHORITY:
	msg="No authority could be contacted for authentication."
	    "The domain name of the authenticating party could be wrong,"
	    " the domain could be unreachable, or there might have been"
	    " a trust relationship failure.";
	break;
      case SEC_E_INSUFFICIENT_MEMORY:
	msg="One or more of the SecBufferDesc structures passed as"
	    " an OUT parameter has a buffer that is too small.";
	break;
      case SEC_E_INVALID_TOKEN:
	msg="The error is due to a malformed input token, such as a"
	    " token corrupted in transit, a token"
	    " of incorrect size, or a token passed into the wrong"
	    " security package. Passing a token to"
	    " the wrong package can happen if client and server did not"
	    " negotiate the proper security package.";
	break;
      default:
	msg = "Internal SSPI error";
	break;
    }

    buf->value = dupstr(msg);
    buf->length = strlen(buf->value);
    
    return SSH_GSS_OK;
}

static Ssh_gss_stat ssh_sspi_get_mic(struct ssh_gss_library *lib,
				     Ssh_gss_ctx ctx, Ssh_gss_buf *buf,
				     Ssh_gss_buf *hash)
{
    winSsh_gss_ctx *winctx= (winSsh_gss_ctx *) ctx;
    SecPkgContext_Sizes ContextSizes;
    SecBufferDesc InputBufferDescriptor;
    SecBuffer InputSecurityToken[2];

    if (winctx == NULL) return SSH_GSS_FAILURE;
  
    winctx->maj_stat = 0;

    memset(&ContextSizes, 0, sizeof(ContextSizes));

    winctx->maj_stat = p_QueryContextAttributesA(&winctx->context,
						 SECPKG_ATTR_SIZES,
						 &ContextSizes);
    
    if (winctx->maj_stat != SEC_E_OK ||
	ContextSizes.cbMaxSignature == 0)
	return winctx->maj_stat;

    InputBufferDescriptor.cBuffers = 2;
    InputBufferDescriptor.pBuffers = InputSecurityToken;
    InputBufferDescriptor.ulVersion = SECBUFFER_VERSION;
    InputSecurityToken[0].BufferType = SECBUFFER_DATA;
    InputSecurityToken[0].cbBuffer = buf->length;
    InputSecurityToken[0].pvBuffer = buf->value;
    InputSecurityToken[1].BufferType = SECBUFFER_TOKEN;
    InputSecurityToken[1].cbBuffer = ContextSizes.cbMaxSignature;
    InputSecurityToken[1].pvBuffer = snewn(ContextSizes.cbMaxSignature, char);

    winctx->maj_stat = p_MakeSignature(&winctx->context,
				       0,
				       &InputBufferDescriptor,
				       0);

    if (winctx->maj_stat == SEC_E_OK) {
	hash->length = InputSecurityToken[1].cbBuffer;
	hash->value = InputSecurityToken[1].pvBuffer;
    }

    return winctx->maj_stat;
}

static Ssh_gss_stat ssh_sspi_free_mic(struct ssh_gss_library *lib,
				      Ssh_gss_buf *hash)
{
    sfree(hash->value);
    return SSH_GSS_OK;
}

static void ssh_sspi_bind_fns(struct ssh_gss_library *lib)
{
    lib->indicate_mech = ssh_sspi_indicate_mech;
    lib->import_name = ssh_sspi_import_name;
    lib->release_name = ssh_sspi_release_name;
    lib->init_sec_context = ssh_sspi_init_sec_context;
    lib->free_tok = ssh_sspi_free_tok;
    lib->acquire_cred = ssh_sspi_acquire_cred;
    lib->release_cred = ssh_sspi_release_cred;
    lib->get_mic = ssh_sspi_get_mic;
    lib->free_mic = ssh_sspi_free_mic;
    lib->display_status = ssh_sspi_display_status;
}

#else

/* Dummy function so this source file defines something if NO_GSSAPI
   is defined. */

void ssh_gss_init(void)
{
}

#endif
