cmake_minimum_required(VERSION 3.10)
project (DataExtractor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# include TGUI library
set(TGUI_BACKEND SFML_GRAPHICS)
set(TGUI_BUILD_FRAMEWORK FALSE)

include(FetchContent)
FetchContent_Declare(
    tgui
    GIT_REPOSITORY https://github.com/texus/TGUI.git
    GIT_TAG 2caf86a8a89c18b8aec6881b88d53cf5c039d4b9
)
FetchContent_MakeAvailable(tgui)

find_package(ZLIB)

include_directories(src)
include_directories(lib)

add_executable(data_extractor
src/main.cpp

src/file_formats/dict.cpp
src/file_formats/data.cpp
src/file_formats/texture.cpp
src/file_formats/bitmap.cpp

src/extracted_files/file_table.cpp
src/extracted_files/texture_metadata_file.cpp
src/extracted_files/mixed_data_file.cpp

src/gui/gui.cpp
)
target_link_libraries(data_extractor Binary-Reader)
target_link_libraries(data_extractor tgui)
target_link_libraries(data_extractor ZLIB::ZLIB)
target_link_libraries(data_extractor ETC1Lib)
target_link_libraries(data_extractor LodePNG)

add_subdirectory(lib/Binary-Reader)
add_subdirectory(lib/ETC1Lib)
add_subdirectory(lib/lodepng)