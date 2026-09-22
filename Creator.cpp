#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <windows.h>
#include "employee.h"

int main(int argc, char* argv[]) {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    try {
        if (argc != 3) {
            throw std::invalid_argument("Использование: Creator <имя_файла> <количество_записей>");
        }

        std::string filename = argv[1];
        int count = std::stoi(argv[2]);

        std::vector<employee> employees(count);

        for (int i = 0; i < count; ++i) {
            std::cout << "Сотрудник " << i + 1 << ":\n";
            std::cout << "  ID: ";
            if (!(std::cin >> employees[i].num)) throw std::invalid_argument("Ошибка ввода ID");

            std::cout << "  Имя (до 9 символов): ";
            std::cin >> employees[i].name;

            std::cout << "  Часы: ";
            if (!(std::cin >> employees[i].hours)) throw std::invalid_argument("Ошибка ввода часов");
        }

        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            throw std::runtime_error("Ошибка создания файла.");
        }

        outFile.write(reinterpret_cast<const char*>(employees.data()), employees.size() * sizeof(employee));

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка Creator: " << e.what() << '\n';
        return 1;
    }

    return 0;
}