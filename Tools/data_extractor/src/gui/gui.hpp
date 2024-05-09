#pragma once

#include <memory>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "file_formats/dict.hpp"
#include "file_formats/data.hpp"

class FileTableEntry;
class TextureMetaData;
class DataFile;

class GUI
{
public:
    GUI();
    ~GUI() = default;

    void StartApplication();

private:
    void LoadMainForm();
    void InitExtractedFilesPanelListBox(const std::vector<DataFile> &extracted_files);

    void ExtractFiles();
    void ExtractTexture(FileTableEntry &texture_entry, TextureMetaData texture_metadata, std::string mixed_data_file_path);

private:
    std::unique_ptr<tgui::Gui> gui;
    tgui::Theme dark_theme;

    tgui::Button::Ptr load_dict_button;
    tgui::Button::Ptr load_data_button;
    tgui::Button::Ptr start_extraction_button;

    tgui::PanelListBox::Ptr extracted_files_panel_list_box;
    tgui::Button::Ptr repack_files_button;

    tgui::String dict_file_path = "/home/laurenz/Dokumente/Programmieren/3DS-Modding/DarkMoon/Tools/data_extractor/resources/data_and_dict_files/boo.dict";
    tgui::String data_file_path = "/home/laurenz/Dokumente/Programmieren/3DS-Modding/DarkMoon/Tools/data_extractor/resources/data_and_dict_files/boo.data";


    std::unique_ptr<Dict> dict;
    std::unique_ptr<Data> data;
};

