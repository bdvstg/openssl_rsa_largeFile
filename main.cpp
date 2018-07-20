#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

#include "file.h"
#include "openssl_rsa.h"
#include <trace/utils.h>

#define TRACE_TAG "rasutl2::main"

using namespace OpenSsl_RsaUtl2;

std::string gInFilename;
std::string gOutFilename;
RSA_Utl::Key_t gKeyType = RSA_Utl::Key_t::pri;
RSA_Utl::Crypt_t gCrypt = RSA_Utl::Crypt_t::dec;
std::string gKeyFilename;
std::string gKeyPassword;

inline bool test_argvList(std::string v, const std::vector<std::string> &list)
{
    for(const auto &p : list)
        if(v == p)
            return true;
    return false;
}

int try_trace_level(std::string v)
{
    try { // guess next argv is verbose number or not
        int dbgLv = std::stoi(v.c_str());
        if(dbgLv >= Trace::LEVEL_ZERO && dbgLv < Trace::LEVEL_MAX) {
            Trace::SetLevel(dbgLv);
            return 1;
        }
        else TRACE_ERROR_THROW("-verbose N, N should be %d - %d",
                Trace::LEVEL_ZERO, Trace::LEVEL_MAX -1);
    }
    catch(const std::invalid_argument &e) { } // it should be another argv
    catch(const std::out_of_range &e)
        TRACE_ERROR_THROW("%s", e.what());
    return 0;
}

static void argsParser(int argc, const char *argv_[])
{
    std::vector<std::string> argv(argc);
    for(int i = 0 ; i < argc ; i++)
        argv[i] = argv_[i];

    for(int i = 1 ; i < argc ; i++)
    {
#define TEST(...) if(test_argvList(argv[i], {__VA_ARGS__} ))
#define ELSE_TEST(...) else TEST(__VA_ARGS__)

        TEST("-inkey")
            gKeyFilename = argv[++i];
        ELSE_TEST("-pri", "-prikey","-privatekey") {
            gKeyFilename = argv[++i];
            gKeyType = RSA_Utl::Key_t::pri;
        }
        ELSE_TEST("-pub", "-pubkey","-publickey") {
            gKeyFilename = argv[++i];
            gKeyType = RSA_Utl::Key_t::pub;
        }
        ELSE_TEST("-pass", "-passphrase")
            gKeyPassword = argv[++i];
        ELSE_TEST("-in","infile")
            gInFilename = argv[++i];
        ELSE_TEST("-out","-outfile")
            gOutFilename = argv[++i];
        ELSE_TEST("-encrypt", "-enc")
            gCrypt = RSA_Utl::Crypt_t::enc; // default Crypt_t::dec
        ELSE_TEST("-decrypt", "-dec")
            gCrypt = RSA_Utl::Crypt_t::dec;
        ELSE_TEST("-pubin")
            gKeyType = RSA_Utl::Key_t::pub; // default Key_t::pri
        ELSE_TEST("-verbose") {
            if( argv.size() > (i+1) )
                i += try_trace_level(argv[i+1]);
            else
                Trace::SetLevel(Trace::Level::LEVEL_DEBUG);
        } else {
            TRACE_ERROR_THROW("unrecognized argument %s", argv_[i]);
        }

#undef ELSE_TEST
#undef TEST
    }
}

int main(int argc, const char *argv[])
{
    Trace::SetLevel(Trace::LEVEL_ZERO);
    argsParser(argc, argv);
    RSA_Utl rsa;
    rsa.init(gKeyFilename, gKeyPassword, gKeyType, gCrypt);

    FILE *file_in = fopen(gInFilename.c_str(), "rb");
    FILE *file_out = fopen(gOutFilename.c_str(), "wb");
    auto fileSize = file_getSize(file_in);
    TRACE_DEBUG("size of in file is %d", fileSize);
    std::vector<char> buf( rsa.MaxDataSize() );
    TRACE_MESSAGE("remaining %15d bytes", fileSize);
    while(fileSize > 0)
    {
        int r = fread(buf.data(), 1, buf.size(), file_in);
        fileSize -= r;
        auto data = rsa.handle(std::vector<char>(buf.begin(), buf.begin() + r));
        file_writeAll(data, file_out);
        TRACE_MESSAGE("remaining %15d bytes", fileSize);
    }
}