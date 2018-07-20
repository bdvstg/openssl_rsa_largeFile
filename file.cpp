#include "file.h"

#include <cassert>

#include <trace/utils.h>

#define TRACE_TAG "rasutl2::file"

namespace OpenSsl_RsaUtl2 {

long file_getSize(FILE *file)
{
    long fileSize;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    return fileSize;
}

std::vector<char> file_readAll(FILE *file)
{
    assert(file != nullptr);
    int fileSize = file_getSize(file);
    std::vector<char> buf(fileSize);
    auto readin = fread(buf.data(), 1, fileSize, file);
    if(readin != fileSize)
        TRACE_ERROR_THROW("error occured when file read");

    fclose(file);
    return buf;
}

std::vector<char> file_readAll(std::string filename)
{
    FILE *file = fopen(filename.c_str(), "rb");
    return file_readAll(file);
}

void file_writeAll(const std::vector<char> &data, FILE *file)
{
    int dataSize = data.size();
    int wrs = 0; // wrote size
    int errCnt = 0;
    const int maxErrCnt = 13;
    while(wrs < dataSize)
    {
        int w = fwrite(data.data() + wrs, 1, data.size() - wrs, file);
        if(w > 0) wrs += w;
        else errCnt++;

        if(errCnt > maxErrCnt)
            TRACE_ERROR_THROW("write failed exceed %d times", errCnt);
    }
}

} // namespace OpenSsl_RsaUtl2