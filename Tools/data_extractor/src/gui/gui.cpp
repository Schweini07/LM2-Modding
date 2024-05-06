#include "gui.hpp"

#include "file_formats/dict.hpp"
#include "file_formats/data.hpp"
#include "file_formats/texture.hpp"
#include "extracted_files/file_table.hpp"
#include "extracted_files/texture_metadata_file.hpp"
#include "extracted_files/mixed_data_file.hpp"

GUI::GUI()
{
}

void GUI::StartApplication()
{
    sf::RenderWindow window(sf::VideoMode(960, 540), "Luigi's Mansion Data Extractor");
    gui = std::make_unique<tgui::Gui>(window);
    tgui::Theme::setDefault("resources/themes/Dark.txt");

    LoadMainForm(); 

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            gui->handleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        gui->draw();

        window.display();
    }
}

void GUI::LoadMainForm()
{
    gui->loadWidgetsFromFile("resources/forms/main_form.txt");

    load_dict_button = gui->get<tgui::Button>("LoadDictButton");
    load_dict_button->onPress([this] {
        tgui::FileDialog::Ptr file_dialog = tgui::FileDialog::create("Select .dict file", "Select", false);
        file_dialog->setFileTypeFilters({
            {"NLG dictionary (*.dict)", {"*.dict"}}
        });
        file_dialog->onFileSelect([this, file_dialog] {
           load_dict_button->setText(file_dialog->getSelectedPaths()[0].getFilename());
           dict_file_path = file_dialog->getSelectedPaths()[0].asString();
        });

        gui->add(file_dialog);
    });

    load_data_button = gui->get<tgui::Button>("LoadDataButton");
    load_data_button->onPress([this] {
        tgui::FileDialog::Ptr file_dialog = tgui::FileDialog::create("Select .data file", "Select", false);
        file_dialog->setFileTypeFilters({
            {"NLG data file (*.data)", {"*.data"}}
        });
        file_dialog->onFileSelect([this, file_dialog] {
           load_data_button->setText(file_dialog->getSelectedPaths()[0].getFilename());
           data_file_path = file_dialog->getSelectedPaths()[0].asString();
        });
        
        gui->add(file_dialog);
    });

    start_extraction_button = gui->get<tgui::Button>("StartExtractionButton");
    start_extraction_button->onPress([this] {
        if (dict_file_path == "" || data_file_path == "")
            return;
        
        ExtractFiles();
    });
}

void GUI::ExtractFiles()
{
    Dict dict(dict_file_path.toStdString());
    dict.ParseDict();

    Data data(data_file_path.toStdString());
    data.ExtractDataFiles(&dict);

    FileTable file_table(dict.file_array[0].file_path);
    file_table.Parse();

    TextureMetaDataFile text_metadata_file(dict.file_array[2].file_path);
    text_metadata_file.Parse();

    for (size_t i = 0; i < file_table.texture_data.size(); i++)
    {
        FileTableEntry texture_entry = file_table.texture_data[i];
        TextureMetaData texture_metadata = text_metadata_file.texture_metadata[i];

        ExtractTexture(texture_entry, texture_metadata, dict.file_array[3].file_path);
    }
}

void GUI::ExtractTexture(FileTableEntry &texture_entry, TextureMetaData texture_metadata, std::string mixed_data_file_path)
{
    MixedDataFile mixed_data_file(mixed_data_file_path);

    Texture texture(
        mixed_data_file.GetDataBuffer(texture_entry.offset, texture_entry.length),
        texture_metadata.width,
        texture_metadata.height,
        false
    );

    texture.DecodeETC1();
    texture.Save(mixed_data_file_path + std::to_string(texture_metadata.hash));
}
