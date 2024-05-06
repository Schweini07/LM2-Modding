#include "data.hpp"

#include "dict.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <zlib.h>

Data::Data(std::string file_path) : file_path(file_path)
{
}

void Data::ExtractDataFiles(Dict *dict)
{
    is_compressed = dict->compression_flag;

    dir_name = "data_files_";
    dir_name += std::filesystem::path(file_path).filename().generic_string();
    std::filesystem::create_directories(dir_name);
    for (DataFile &data_file : dict->file_array)
    {
        ExtractDataFile(data_file);

        if (is_compressed)
            DecompressDataFile(data_file);
    }
}

void Data::ExtractDataFile(DataFile &data_file)
{
    data_file.file_path = dir_name + "/file" + std::to_string(data_file.id);
    std::cout << data_file.file_path << "\n";
    std::fstream new_file(data_file.file_path, std::fstream::out);

    std::fstream data(file_path, std::fstream::in);
    if (!data)
    {
        std::cerr << "Couldn't open .data file!\n";
        return;
    }

    uint32_t data_file_size = is_compressed ? data_file.compressed_file_length : data_file.decompressed_file_length;
    
    data.seekg(data_file.offset, std::ios_base::beg);

    std::cout << int(data.tellg()) - data_file.offset << "\n";
    char data_buffer[data_file_size];
    data.read(data_buffer, data_file_size);

    new_file.write(data_buffer, data_file_size);

    data.close();
    new_file.close();
}

void Data::DecompressDataFile(DataFile &data_file)
{
    char compressed_data[data_file.compressed_file_length];
    char decompressed_data[data_file.decompressed_file_length];
    uLongf decompressed_size = data_file.decompressed_file_length;
    uLongf compressed_size = data_file.compressed_file_length;

    std::ifstream compressed_file(data_file.file_path);
    compressed_file.read(compressed_data, data_file.compressed_file_length);
    compressed_file.close();

    uncompress((Bytef *)decompressed_data, &decompressed_size, (Bytef *)compressed_data, compressed_size);

    std::ofstream decompressed_file(data_file.file_path);
    decompressed_file.write(decompressed_data, decompressed_size);
    decompressed_file.close();
}
 