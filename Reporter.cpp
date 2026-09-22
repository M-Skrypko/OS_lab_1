#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <windows.h>
#include "employee.h"

int main(int argc, char* argv[]) {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    try {
        if (argc != 4) {
            throw std::invalid_argument("Использование: Reporter <входной_бинарный_файл> <файл_отчета> <оплата_за_час>");
        }

        std::string binFilename = argv[1];
        std::string reportFilename = argv[2];
        double hourlyWage = std::stod(argv[3]);

        std::ifstream inFile(binFilename, std::ios::binary);
        if (!inFile) {
            throw std::runtime_error("Ошибка открытия бинарного файла.");
        }

        std::vector<employee> employees;
        employee emp;
        while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
            employees.push_back(emp);
        }
        inFile.close();

        std::sort(employees.begin(), employees.end(), [](const employee& a, const employee& b) {
            return a.num < b.num;
            });

        std::ofstream outFile(reportFilename);
        if (!outFile) {
            throw std::runtime_error("Ошибка создания файла отчета.");
        }

        outFile << "Отчет по файлу \"" << binFilename << "\"\n";
        outFile << "Номер сотрудника, имя сотрудника, часы, зарплата\n";

        for (const auto& e : employees) {
            double salary = e.hours * hourlyWage;
            outFile << e.num << ", " << e.name << ", " << e.hours << ", "
                << std::fixed << std::setprecision(2) << salary << "\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка Reporter: " << e.what() << '\n';
        return 1;
    }

    return 0;
}