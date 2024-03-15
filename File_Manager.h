#pragma once

class File_Manager
{
private:
private:
    Menu menu;
    string current_directory;
public:
    File_Manager(const string& start_directory) : menu({}), current_directory(start_directory) {}

    void showMenu();
    void openFile(const string& filename);
    void deleteFile(const string& filename);
    void renameFile(const string& oldName, const string& newName);
    void createFolder(const string& folderName);
    void createFile(const string& filename);
    void navigateUp();
    void navigateDown(const string& folderName);
    void refreshMenu();
};

void File_Manager::showMenu() {
    menu.load_options(current_directory);
    menu.display();

    int choiceIndex = menu.get_choice() - 1;
    if (choiceIndex >= 0 && choiceIndex < menu.get_options().size()) {
        string selectedOption = current_directory + "/" + menu.get_options()[choiceIndex];
        if (filesystem::is_directory(selectedOption)) {
            navigateDown(selectedOption);
        }
        else {
            openFile(selectedOption);
        }
    }
}

void File_Manager::openFile(const string& file_name) 
{
    const string current_file_path = menu.get_current_path() + file_name;

    wstring wideFilePath(current_file_path.begin(), current_file_path.end());
    LPCWSTR lpcwstr_current_file_path = wideFilePath.c_str();

    ShellExecute(NULL, L"open", lpcwstr_current_file_path, NULL, NULL, SW_SHOWNORMAL);
}

void File_Manager::deleteFile(const string& filename) {
    //TODO Удалить файл
}

void File_Manager::renameFile(const string& oldName, const string& newName) {
    //TODO Переименовать файл
}

void File_Manager::createFolder(const string& folderName) {
    string newFolderPath = current_directory + "/" + folderName;
    filesystem::create_directory(newFolderPath);
    refreshMenu();
}

void File_Manager::createFile(const string& filename) {
    string newFilePath = current_directory + "/" + filename;
    ofstream newFile(newFilePath);
    newFile.close();
    refreshMenu();
}

void File_Manager::navigateUp() {
    if (current_directory != "/") {
        current_directory = current_directory.substr(0, current_directory.find_last_of("/"));
        refreshMenu();
    }
}

void File_Manager::navigateDown(const string& folderName) {
    current_directory = folderName;
    refreshMenu();
}

void File_Manager::refreshMenu() {
    menu.load_options(current_directory);
    menu.display();
}

