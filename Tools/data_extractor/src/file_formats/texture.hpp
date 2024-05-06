#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <memory>

class Texture
{
public:
    Texture(std::vector<char> texture_data, uint16_t width, uint16_t height, bool alpha);
    ~Texture() = default;

    void DecodeETC1();
    void Save(std::string file_path);

private:
    std::vector<char> texture_data;
    std::unique_ptr<unsigned int[]> decoded_data;

    uint16_t width;
    uint16_t height;
    bool alpha;

    unsigned decoded_data_size;
};
