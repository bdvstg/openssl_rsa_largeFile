#include "openssl_rsa.h"

#include <cassert>
#include <map>
#include <string.h>

#ifdef  __cplusplus
extern  "C" {
#endif

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#ifdef  __cplusplus
}
#endif 

#include "file.h"
#include <trace/utils.h>

#define TRACE_TAG "rasutl2::rsa"

namespace OpenSsl_RsaUtl2 {


static int f_Password_cb(char *buf, int size, int rwflag, void *userdata)
{// rwflag indicates whether the callback is used for reading/decryption (rwflag = 0) or writing/encryption (rwflag = 1).
    std::string *passwd = (std::string*)userdata;
    TRACE_DEBUG("    password : buf_size(%d), rwflag(%d)", size, rwflag);
    if(!passwd->empty() && passwd->length() < size)
    {
        TRACE_DEBUG("    using passphrase \"%s\"", passwd->c_str());
        memcpy(buf, passwd->c_str(), passwd->length());
        return passwd->length();
    }
    else return 0;
}

// The RSAPrivateKey functions process an RSA private key using an RSA
// structure. The write routines uses traditional format. The read routines
// handles the same formats as the PrivateKey functions but an error occurs
// if the private key is not RSA.
//RSA *PEM_read_bio_RSAPrivateKey(BIO *bp, RSA **x, pem_password_cb *cb, void *u);

// The RSAPublicKey functions process an RSA public key using an RSA structure.
// The public key is encoded using a PKCS#1 RSAPublicKey structure.
//RSA *PEM_read_bio_RSAPublicKey(BIO *bp, RSA **x, pem_password_cb *cb, void *u);

// The RSA_PUBKEY functions also process an RSA public key using an RSA
// structure. However the public key is encoded using a SubjectPublicKeyInfo
// structure and an error occurs if the public key is not RSA.
//RSA *PEM_read_bio_RSA_PUBKEY(BIO *bp, RSA **x, pem_password_cb *cb, void *u);

void RSA_Utl::init(std::string key_filename, std::string key_password,
            Key_t key_type, Crypt_t crypt_type)
{
    mKeyType = key_type;
    mCrypt = crypt_type;
    mKeyFilename = key_filename;
    mKeyPassword = key_password;

    TRACE_DEBUG("reading inkey %s", mKeyFilename.c_str());
    auto key_Content = file_readAll(mKeyFilename);

    TRACE_DEBUG("Create BIO");
    rsa_st *rsa= nullptr;

    BIO *keybio = BIO_new_mem_buf(key_Content.data(), -1);
    if (keybio == NULL)
        TRACE_ERROR_THROW("    Failed to create key BIO");

    TRACE_DEBUG("Init RSA");
    if(mKeyType == Key_t::pub)
        rsa = PEM_read_bio_RSA_PUBKEY(
            keybio, &rsa, f_Password_cb, &mKeyPassword);
    else if (mKeyType == Key_t::pri)
        rsa = PEM_read_bio_RSAPrivateKey(
            keybio, &rsa, f_Password_cb, &mKeyPassword);
    else
        throw "unexpect error";

    if(rsa == NULL)
        TRACE_ERROR_THROW("Failed to create RSA");

    mRsaModSize = RSA_size(rsa);
    mRsaBuffer.resize(mRsaModSize);
    mRsa = rsa;
    TRACE_DEBUG("RSA_size = %d", mRsaModSize);
}

//int RSA_public_encrypt (int flen, unsigned char *from, unsigned char *to, RSA *rsa, int padding);
//int RSA_public_decrypt (int flen, unsigned char *from, unsigned char *to, RSA *rsa, int padding);
//int RSA_private_encrypt(int flen, unsigned char *from, unsigned char *to, RSA *rsa, int padding);
//int RSA_private_decrypt(int flen, unsigned char *from, unsigned char *to, RSA *rsa, int padding);
using low_level_handle_t = std::function<
    int(int, unsigned char*, unsigned char*, RSA *rsa, int)
    >;
using which_t = std::pair<RSA_Utl::Key_t, RSA_Utl::Crypt_t>;
std::map<which_t, low_level_handle_t> low_level_handles = {
    {{RSA_Utl::Key_t::pub, RSA_Utl::Crypt_t::enc}, RSA_public_encrypt},
    {{RSA_Utl::Key_t::pub, RSA_Utl::Crypt_t::dec}, RSA_public_decrypt},
    {{RSA_Utl::Key_t::pri, RSA_Utl::Crypt_t::enc}, RSA_private_encrypt},
    {{RSA_Utl::Key_t::pri, RSA_Utl::Crypt_t::dec}, RSA_private_decrypt},
};

std::vector<char> RSA_Utl::handle(std::vector<char> data)
{
    assert(mRsa != nullptr);
    assert(!mRsaBuffer.empty());

    int result = low_level_handles[{mKeyType, mCrypt}](
        data.size(),
        (unsigned char *)data.data(),
        mRsaBuffer.data(),
        mRsa, RSA_PKCS1_PADDING);

    if(result == -1) {
        auto errnum = ERR_get_error();
        char * err = (char*)malloc(1024);;
        ERR_load_crypto_strings();
        ERR_error_string_n(errnum, err, 1024);
        TRACE_ERROR("handle : %s", err);
        free(err);
        exit(1);
    }

    return std::vector<char>(
        mRsaBuffer.begin(),
        mRsaBuffer.begin() + result);
}

int RSA_Utl::MaxDataSize() const
{// we use RSA_PKCS1_PADDING in handle, so minus 11
    TRACE_ASSERT( (mRsaModSize - 11) > 0 );
    return (mRsaModSize - 11);
}

} // namespace OpenSsl_RsaUtl2
