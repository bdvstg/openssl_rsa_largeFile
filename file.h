#ifndef OPENSSL_RSAUTL2_FILE_H__
#define OPENSSL_RSAUTL2_FILE_H__

#include <stdio.h>
#include <string>
#include <vector>

namespace OpenSsl_RsaUtl2 {

    long file_getSize(FILE *file);
    std::vector<char> file_readAll(FILE *file);
    std::vector<char> file_readAll(std::string filename);
    void file_writeAll(const std::vector<char> &data, FILE *file);

} // namespace OpenSsl_RsaUtl2

#endif // OPENSSL_RSAUTL2_FILE_H__
