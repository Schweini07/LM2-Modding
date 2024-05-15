#include "data.hpp"

#include "dict.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <zlib.h>

Data::Data(std::string file_path) : file_path(file_path)
{
    std::ifstream data_file(file_path, std::ifstream::ate);
    file_size = data_file.tellg();
    data_file.close();

    std::cout << file_size << "\n";
}

void Data::GetDataBuffer(std::vector<uint8_t> &data_buffer, uint32_t offset, uint32_t buffer_size)
{
    data_buffer.resize(buffer_size);

    std::ifstream data_file(file_path);

    data_file.seekg(offset);
    data_file.read(reinterpret_cast<char *>(data_buffer.data()), buffer_size);

    data_file.close();
}

uint32_t Data::GetFileSize()
{
    return file_size;
}

void Data::ExtractDataFiles(Dict *dict)
{
    is_compressed = dict->compression_flag;

    dir_name = "data_files_";
    dir_name += std::filesystem::path(file_path).filename().generic_string();
    std::filesystem::create_directories(dir_name);
    for (FileSection &data_file : dict->file_array)
    {
        ExtractDataFile(data_file);

        if (is_compressed)
            DecompressDataFile(data_file);
    }
}

void Data::RepackDataFiles(Dict *dict)
{
    std::ofstream repacked_file(file_path + ".repacked");
    for (FileSection &data_file : dict->file_array)
    {
        if (is_compressed)
            CompressDataFile(data_file);

        uint32_t data_file_size = is_compressed ? data_file.compressed_file_length : data_file.decompressed_file_length;
        std::cout << "File Size: " << data_file_size << "\n";
        char data[data_file_size];
        std::ifstream extracted_file(data_file.file_path + ".repacked");
        extracted_file.read(data, data_file_size);
        extracted_file.close();

        repacked_file.seekp(data_file.offset);

        repacked_file.write(data, data_file_size);
    }
    repacked_file.close();
}

void Data::ExtractDataFile(FileSection &data_file)
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

void Data::DecompressDataFile(FileSection &data_file)
{
    char compressed_data[data_file.compressed_file_length];
    uLongf compressed_size = data_file.compressed_file_length;
    char decompressed_data[data_file.decompressed_file_length];
    uLongf decompressed_size = data_file.decompressed_file_length;

    std::ifstream compressed_file(data_file.file_path);
    compressed_file.read(compressed_data, compressed_size);
    compressed_file.close();

    uncompress((Bytef *)decompressed_data, &decompressed_size, (Bytef *)compressed_data, compressed_size);

    std::ofstream decompressed_file(data_file.file_path);
    decompressed_file.write(decompressed_data, decompressed_size);
    decompressed_file.close();
}

void Data::CompressDataFile(FileSection &data_file)
{
    char decompressed_data[data_file.decompressed_file_length];
    uLongf decompressed_size = data_file.decompressed_file_length;
    char compressed_data[data_file.compressed_file_length];
    uLongf compressed_size = data_file.compressed_file_length;
    
    std::ifstream decompressed_file(data_file.file_path);
    decompressed_file.read(decompressed_data, decompressed_size);
    decompressed_file.close();

    compress((Bytef *)compressed_data, &compressed_size, (Bytef *)decompressed_data, decompressed_size);

    std::ofstream compressed_file(data_file.file_path + ".repacked");
    compressed_file.write(compressed_data, compressed_size);
    compressed_file.close();
}
