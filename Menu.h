#pragma once

class Menu
{
	vector<string> options;
	vector<string> path;
	int current_option;
public:
    Menu() : current_option(0) {}
	Menu(const vector<string>& menu_options) : options(menu_options), current_option(0) {}

    void display();
    void load_options(const string& directory);
    int get_choice();
    const vector<string>& get_options() const;
};

const vector<string>& Menu::get_options() const {
    return options;
}



void Menu::display()
{
    system("cls"); // Очистить экран
    cout << filesystem::current_path() << '\n';
    for (int i = 0; i < options.size(); ++i) {
        if (i == current_option) {
            text_color(Green); // Установить цвет выбранного пункта
            cout << options[i] << "  <<\n";
        }
        else {
            text_color(15); // Установить цвет обычного текста
            cout << options[i] << '\n';
        }
    }// Отобразить кнопку создания папки/файла
    text_color(7); // Установить цвет обычного текста

    cout << "\x1b[s\x1b[1;80H<-Q\x1b[u";

    cout << "\x1b[s\x1b[2;80H";

    if (current_option == -1 || current_option == -2) {
        text_color(Green);
    }
     cout << "Create:";
     switch (current_option) {
        case -2: text_color(7); cout << "D->Folder "; text_color(Blue); cout << "F->File"; text_color(7); break;
        case -1:  text_color(Blue); cout << "D->Folder "; text_color(7); cout << "F->File"; break;
        default:cout << "D->Folder F->File";
     }
    cout<< "\x1b[u";
}

void Menu::load_options(const string& directory) {
    options.clear();
    path.clear();
    current_option = 0;

    try {
        // Проверяем, существует ли каталог и доступен ли он для чтения
        if (!filesystem::exists(directory) || !filesystem::is_directory(directory)) {
            cerr << "Error: Directory does not exist or is not accessible.\n";
            return;
        }

        for (const auto& entry : filesystem::directory_iterator(directory)) {
            options.push_back(entry.path().filename().string());
        }
    }
    catch (const filesystem::filesystem_error& e) {
        cerr << "Error: " << e.what() << '\n';
    }
}

int Menu::get_choice() {
    int choice = 0;
    while (true) {
        if (_kbhit()) {
            char key = _getch();
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
            else if (key == 'D' || key == 'd') { // Создание файла
                current_option = -1;
            }
            else if (key == 'F' || key == 'f') { // Создание папки
                current_option = -2;
            }
            else if (key == 'Q' || key == 'q') {//откат на шаг назад
                choice = current_option = -3;
                break;
            }
            display();
        }
    }
    return choice;
}