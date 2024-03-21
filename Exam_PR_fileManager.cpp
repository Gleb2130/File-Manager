#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <vector>
#include <iomanip>
#include <string>
#include <conio.h>
using namespace std;

#include "Console_color.h"
#include "Menu.h"
#include "File_Manager.h"

int main()
    {
    string startDirectory = "C:\\";

    // Создание экземпляра менеджера файлов
    File_Manager file_manager(startDirectory);

    do {
        // Показываем меню и ожидаем выбора пользователя
    } while (!file_manager.show_menu());
}