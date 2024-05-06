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
    void ExtractDataFile(DataFile &data_file);
    void DecompressDataFile(DataFile &data_file);

    bool is_compressed;

private:
    std::string file_path;
    std::string dir_name;
};
