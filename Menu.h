#pragma once

#include "Console_color.h"

class Menu
{
	vector<string> options;
	vector<string> path;
	int current_option;
public:
	Menu(const vector<string>& menu_options) : options(menu_options), current_option(0) {}

    void display();
    void update_options();
    void load_options(const string& directory);
    int get_choice();
    void add_to_path(const string& folder_name);
    void remove_from_path();
    string get_current_path() const;
    const vector<string>& get_options() const;
};

const vector<string>& Menu::get_options() const {
    return options;
}

void Menu::display()
{
    system("cls"); // Очистить экран
    for (int i = 0; i < options.size(); ++i) {
        if (i == current_option) {
            text_color(15); // Установить цвет выбранного пункта
        }
        else {
           text_color(7); // Установить цвет обычного текста
        }
        cout << options[i] << '\n';
    }// Отобразить кнопку создания папки/файла
    text_color(7); // Установить цвет обычного текста
    cout << "Create Folder/File" << '\n';

}


void Menu::load_options(const string& directory) {
    options.clear();
    path.clear();
    for (const auto& entry : filesystem::directory_iterator(directory)) {
        options.push_back(entry.path().filename().string());
    }
    current_option = 0;
}


void Menu::update_options() {
    current_option = 0;
}


int Menu::get_choice() {
    int choice = 0;
    while (true) {
        if (kbhit()) {
            char key = getch();
            if (key == 13) { // Нажата клавиша Enter
                choice = current_option;
                break;
            }
            else if (key == 72 && current_option > 0) { // Нажата клавиша вверх
                current_option--;
            }
            else if (key == 80 && current_option < options.size() - 1) { // Нажата клавиша вниз
                current_option++;
            }
            display();
        }
    }
    return choice;
}


void Menu::add_to_path(const string& folder_name) {
    path.push_back(folder_name);
}

void Menu::remove_from_path() {
    path.pop_back();
}

string Menu::get_current_path() const {
    string current_path = "";
    for (const auto& folder : path) {
        current_path += folder + "/";
    }
    return current_path;
}