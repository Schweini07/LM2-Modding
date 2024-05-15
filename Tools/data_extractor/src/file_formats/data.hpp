#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Dict;
class FileSection;

class Data
{
public:
    Data(std::string file_path);
    ~Data() = default;

    void GetDataBuffer(std::vector<uint8_t> &data_buffer, uint32_t offset, uint32_t buffer_size);
    uint32_t GetFileSize();

    void ExtractDataFiles(Dict *dict);
    void RepackDataFiles(Dict *dict);

    bool is_compressed;

private:
    void ExtractDataFile(FileSection &data_file);
    void DecompressDataFile(FileSection &data_file);
    void CompressDataFile(FileSection &data_file);

    uint32_t file_size;

    std::string file_path;
    std::string dir_name;
};
