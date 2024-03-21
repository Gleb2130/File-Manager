#pragma once

class File_Manager
{
private:
    Menu menu;
    string current_directory;
    size_t file_copy_number(const string& folder_path, const string& file_folder_name) {
        size_t counter = 1;
        for (const auto& entry : filesystem::directory_iterator(folder_path)) {
            if(entry.path().filename().string() == file_folder_name)
                counter++;
        }
        return counter;
    }
public:
    File_Manager(const string& start_directory) : menu({}), current_directory(start_directory) { filesystem::current_path(current_directory); }

    bool show_menu();
    void open_file(const string&);
    void delete_file(const string&);
    void rename_file_folder(const string&, const string&);
    void create_folder(const string&);
    void create_file(const string&);
    void navigate_up();
    void navigate_down(const string&);
    void refresh_menu();

    void open_folder(const string&);
    void delete_folder(const string&);

    void copyFile(const string&,const string&);
    void copyFolder(const string&);
};


void File_Manager::copyFile(const string& source_file_path,const string& file_name) {
    const char copy_text = file_copy_number(source_file_path, file_name);
    std::filesystem::copy_file(source_file_path + '\\' + file_name, source_file_path + "\\(copy - " + copy_text+')' + file_name);
}

//const auto& entry : fs::recursive_directory_iterator(directoryPath)
//fs::is_regular_file(entry.path())
//fs::file_size(entry.path())
//fs::is_regular_file(entry.path()) && entry.path().filename().string().find(mask) != std::string::npos

void File_Manager::delete_folder(const string& folder_path) {
    try {
        filesystem::remove_all(folder_path);
        cout << "Folder deleted successfully.\n";
    }
    catch (const filesystem::filesystem_error& error) {
        cerr << "Error deleting folder: " << error.what() << "\n";
    }
}
void File_Manager::delete_file(const string& file_path) {
    if (filesystem::remove(file_path)) {
        cout << "Error deleting file!\n";
    }
    else {
        cout << "File deleted successfully.\n";
    }
}

inline void File_Manager::rename_file_folder(const string& old_name_path, const string& new_name)
{
    try {
        filesystem::rename(old_name_path, current_directory +"\\"+ new_name);
        cout << "File renaming successfully.\n";
    }
    catch (filesystem::filesystem_error& error)
    {
        cout << "Error renaming: " << error.what() << "\n";
    }
}

void File_Manager::open_folder(const string& folder_path) {
    current_directory = folder_path;
}

void File_Manager::open_file(const string& file_path)
{
    wstring wide_file_path(file_path.begin(), file_path.end());
    LPCWSTR lpcwstr_current_file_path = wide_file_path.c_str();

    ShellExecute(NULL, L"open", lpcwstr_current_file_path, NULL, NULL, SW_SHOWNORMAL);
}

void File_Manager::create_folder(const string& folder_name) {
    string new_folder_path = current_directory +'\\'+ folder_name;
    filesystem::create_directory(new_folder_path);
    refresh_menu();
}

void File_Manager::create_file(const string& file_name) {
    string newIfile_path = current_directory +'\\'+ file_name;
    ofstream new_file(newIfile_path);
    new_file.close();
    refresh_menu();
}

void File_Manager::navigate_up() {
    if (current_directory != "C:\\") {
        current_directory = current_directory.substr(0, current_directory.find_last_of("\\"));
        refresh_menu();
    }
}

void File_Manager::navigate_down(const string& folder_name) {
    current_directory = folder_name;
    filesystem::current_path(current_directory); // Установка текущего рабочего каталога
    refresh_menu();
}


void File_Manager::refresh_menu() {
    menu.load_options(current_directory);
    menu.display();
}

bool File_Manager::show_menu() {
    menu.load_options(current_directory);
    menu.display();

    int choice_index = menu.get_choice();
    if (choice_index >= 0 && choice_index < menu.get_options().size()+2) {
        string selected_option = menu.get_options()[choice_index];
        string selected_item_path = current_directory + "\\" + selected_option;

        // Пользователь выбрал папку
        cout << "Selected folder: " << selected_option << endl;
        cout << "Press Enter to view options..." << endl;

        // Ожидаем нажатия Enter
        while (_getch() != 13);

        // Отображаем подменю для папки
        system("cls");
        cout << "Options for " << selected_option << ":" << endl;
        cout << "-> 1. Open" << endl;
        cout << "2. Rename" << endl;
        cout << "3. Delete" << endl;
        cout << "Press Q to return to main menu" << endl;

        char option = '1';
        char key;
        do {
            key = _getch();
            if (key == 72 && option > '1') {
                --option;
            }
            else if (key == 80 && option < '3') {
                ++option;
            }
            else if (key == 'Q' || key == 'q') {
                return false;
            }

            system("cls");
            cout << "Options for " << selected_option << ":" << endl;
            cout << (option == '1' ? "-> " : "   ") << "1. Open" << endl;
            cout << (option == '2' ? "-> " : "   ") << "2. Rename" << endl;
            cout << (option == '3' ? "-> " : "   ") << "3. Delete" << endl;
            cout << "Press Q to return to main menu" << endl;
        } while (key != 13);

        // Обработка выбора пользователя
        switch (option) {
        case '1':
            filesystem::is_directory(selected_item_path) ? navigate_down(selected_item_path) : open_file(selected_item_path);
            break;
        case '2': {
            string newName;
            cout << "Enter new name: ";
            cin >> newName;
            rename_file_folder(selected_item_path, newName);
            break;
        }
        case '3':
            filesystem::is_directory(selected_item_path) ? delete_folder(selected_item_path) : delete_file(selected_item_path);
            break;
        default:
            cout << "Invalid option!\n";
        }
        return false;  // Продолжаем выполнение программы
    }
    else if (choice_index == -1) { // Создание файла
        cout << "Enter folder name:";
        string folder_name;
        cin >> folder_name;
        create_folder(folder_name);
        return false;
    }
    else if (choice_index == -2) { // Создание папки
        cout << "Enter file name:";
        string file_name;
        cin >> file_name;
        create_file(file_name);
        return false;
    }
    else if (choice_index == -3) {
        navigate_up();
        return false;
    }
}

