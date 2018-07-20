#ifndef OPENSSL_RSAUTL2_OPENSSL_RSA_H__
#define OPENSSL_RSAUTL2_OPENSSL_RSA_H__

#include <memory>
#include <string>
#include <vector>


struct rsa_st;
typedef struct rsa_st RSA;


namespace OpenSsl_RsaUtl2 {

    class RSA_Utl
    {
    public:
        enum class Key_t { pub, pri, };
        enum class Crypt_t { enc, dec, };

    private:
        Key_t mKeyType;
        Crypt_t mCrypt;
        std::string mKeyFilename;
        std::string mKeyPassword;

        std::shared_ptr<RSA> mRsa;
        int mRsaModSize;
        std::vector<unsigned char> mRsaBuffer;

    public:
        RSA_Utl()
            : mRsa(nullptr)
            , mRsaModSize(0)
        {}

        void init(std::string key_filename, std::string key_password,
            Key_t key_type, Crypt_t crypt_type);

        std::vector<char> handle(std::vector<char> data);

        Key_t KeyType() const { return mKeyType; }
        Crypt_t Crypt() const { return mCrypt; }
        int ModSize() const { return mRsaModSize; }
        int MaxDataSize() const;
    };

} // namespace OpenSsl_RsaUtl2

#endif