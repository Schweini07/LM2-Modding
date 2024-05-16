#include "gui.hpp"

#include "file_formats/texture.hpp"
#include "extracted_files/file_table.hpp"
#include "extracted_files/texture_metadata_file.hpp"
#include "extracted_files/mixed_data_file.hpp"

GUI::GUI()
{
    dark_theme.load("resources/themes/Dark.txt");
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

    directory_path_edit_box = gui->get<tgui::EditBox>("DirectoryPathEditBox");
    directory_path_edit_box->onTextChange([this] {
        destination_directory_path = directory_path_edit_box->getText();
    });

    directory_path_button = gui->get<tgui::Button>("DirectoryPathButton");
    directory_path_button->onPress([this] {
        tgui::FileDialog::Ptr file_dialog = tgui::FileDialog::create("Choose directory", "Select", false);
        file_dialog->setSelectingDirectory(true);

        file_dialog->onFileSelect([this, file_dialog] {
            directory_path_edit_box->setText(file_dialog->getSelectedPaths()[0].asString());
        });

        gui->add(file_dialog);
    });

    start_extraction_button = gui->get<tgui::Button>("StartExtractionButton");
    start_extraction_button->onPress([this] {
        if (dict_file_path == "" || data_file_path == "" || destination_directory_path == "")
            return;
        
        ExtractFiles();
    });


    extracted_files_panel_list_box = gui->get<tgui::PanelListBox>("ExtractedFilesPanelListBox");
    extracted_files_panel_list_box->setRenderer(dark_theme.getRenderer("Panel"));

    repack_files_button = gui->get<tgui::Button>("RepackFilesButton");
    repack_files_button->onPress([this] {
        dict_data_manager->RepackFiles();
    });
}

void GUI::InitExtractedFilesPanelListBox(const std::vector<FileSection> &extracted_files)
{
    for (const FileSection &data_file : extracted_files)
    {
        // Conversion is needed, because else no number is displayed for some reason
        tgui::String id(data_file.id);
        tgui::String file_name = "File" + id;

        tgui::Panel::Ptr item_panel = extracted_files_panel_list_box->addItem(id);

        tgui::Label::Ptr file_name_label = tgui::Label::create(file_name);
        item_panel->add(file_name_label);
    }
}

void GUI::ExtractFiles()
{
    dict_data_manager = std::make_unique<DictDataManager>(dict_file_path.toStdString(), data_file_path.toStdString(), destination_directory_path.toStdString());
    dict_data_manager->ExtractFiles();

/*     FileTable file_table(dict->file_array[0].file_path);
    file_table.Parse();

    TextureMetaDataFile text_metadata_file(dict->file_array[2].file_path);
    text_metadata_file.Parse();

    for (size_t i = 0; i < file_table.texture_data.size(); i++)
    {
        FileTableEntry texture_entry = file_table.texture_data[i];
        TextureMetaData texture_metadata = text_metadata_file.texture_metadata[i];

        ExtractTexture(texture_entry, texture_metadata, dict->file_array[3].file_path);
    } */

    //InitExtractedFilesPanelListBox(dict->file_array);
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
