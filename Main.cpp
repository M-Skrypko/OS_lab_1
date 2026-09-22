#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include "employee.h"

std::string getExeDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string path(buffer);
    size_t pos = path.find_last_of("\\/");
    return (pos != std::string::npos) ? path.substr(0, pos + 1) : "";
}

void printBinaryFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) return;
    std::cout << "\n--- Содержимое бинарного файла ---\n";
    employee emp;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        std::cout << "ID: " << emp.num << ", Имя: " << emp.name
            << ", Часы: " << emp.hours << "\n";
    }
    std::cout << "----------------------------------\n\n";
    inFile.close();
}

void printTextFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) return;
    std::cout << "\n--- Содержимое файла отчета ---\n";
    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << "\n";
    }
    std::cout << "-------------------------------\n\n";
    inFile.close();
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    std::string exeDir = getExeDirectory();

    std::string binFilename;
    int recordCount;
    std::cout << "Введите имя бинарного файла: ";
    std::cin >> binFilename;
    std::cout << "Введите количество записей: ";
    std::cin >> recordCount;

    STARTUPINFOA siCreator{};
    PROCESS_INFORMATION piCreator{};
    siCreator.cb = sizeof(siCreator);

    std::string creatorCmd = "\"" + exeDir + "Creator.exe\" \"" + binFilename + "\" " + std::to_string(recordCount);
    std::vector<char> cmdCreatorBuf(creatorCmd.begin(), creatorCmd.end());
    cmdCreatorBuf.push_back('\0');

    if (!CreateProcessA(NULL, cmdCreatorBuf.data(), NULL, NULL, FALSE, 0, NULL, NULL, &siCreator, &piCreator)) {
        std::cerr << "Ошибка запуска Creator.exe: " << GetLastError() << "\n";
        return 1;
    }

    HANDLE hCreator = piCreator.hProcess;
    WaitForSingleObject(hCreator, INFINITE);

    CloseHandle(piCreator.hProcess);
    CloseHandle(piCreator.hThread);

    printBinaryFile(binFilename);

    std::string reportFilename;
    double hourlyWage;
    std::cout << "Введите имя файла отчета: ";
    std::cin >> reportFilename;
    std::cout << "Введите оплату за час работы: ";
    std::cin >> hourlyWage;

    STARTUPINFOA siReporter{};
    PROCESS_INFORMATION piReporter{};
    siReporter.cb = sizeof(siReporter);

    std::string reporterCmd = "\"" + exeDir + "Reporter.exe\" \"" + binFilename + "\" \"" + reportFilename + "\" " + std::to_string(hourlyWage);
    std::vector<char> cmdReporterBuf(reporterCmd.begin(), reporterCmd.end());
    cmdReporterBuf.push_back('\0');

    if (!CreateProcessA(NULL, cmdReporterBuf.data(), NULL, NULL, FALSE, 0, NULL, NULL, &siReporter, &piReporter)) {
        std::cerr << "Ошибка запуска Reporter.exe: " << GetLastError() << "\n";
        return 1;
    }

    HANDLE hReporter = piReporter.hProcess;
    WaitForSingleObject(hReporter, INFINITE);

    CloseHandle(piReporter.hProcess);
    CloseHandle(piReporter.hThread);

    printTextFile(reportFilename);

    return 0;
}