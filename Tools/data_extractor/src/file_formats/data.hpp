#pragma once

#include <string>
#include <vector>

class Dict;
class DataFile;

class Data
{
public:
    Data(std::string file_path);
    ~Data() = default;

    void ExtractDataFiles(Dict *dict);
    void RepackDataFiles(Dict *dict);

    bool is_compressed;

private:
    void ExtractDataFile(DataFile &data_file);
    void DecompressDataFile(DataFile &data_file);
    void CompressDataFile(DataFile &data_file);

    std::string file_path;
    std::string dir_name;
};
