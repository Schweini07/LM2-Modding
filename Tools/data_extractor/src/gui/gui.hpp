#pragma once

#include <memory>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

class FileTableEntry;
class TextureMetaData;

class GUI
{
public:
    GUI();
    ~GUI() = default;

    void StartApplication();

private:
    void LoadMainForm();

    void ExtractFiles();
    void ExtractTexture(FileTableEntry &texture_entry, TextureMetaData texture_metadata, std::string mixed_data_file_path);

private:
    std::unique_ptr<tgui::Gui> gui;

    tgui::Button::Ptr load_dict_button;
    tgui::Button::Ptr load_data_button;
    tgui::Button::Ptr start_extraction_button;

    tgui::String dict_file_path = "/home/laurenz/Dokumente/Programmieren/3DS-Modding/DarkMoon/Tools/data_extractor/resources/data_and_dict_files/room_entrance.dict";
    tgui::String data_file_path = "/home/laurenz/Dokumente/Programmieren/3DS-Modding/DarkMoon/Tools/data_extractor/resources/data_and_dict_files/room_entrance.data";
};

