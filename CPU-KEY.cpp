#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <shlobj.h> // Для OpenFileDialog
#include <intrin.h>  // Для Windows

using namespace std;

//Функция для преоброзования Wchar в std:string
std::string wcharToString(const std::wstring& wstr) {
    // Получаем необходимый размер буфера для хранения строки в кодировке UTF-8
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);

    // Создаем буфер необходимого размера
    std::string strTo(size_needed, 0);

    // Выполняем преобразование из wchar_t в UTF-8
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);

    return strTo;
}

// Функция для получения хеша процессора
uint64_t GetProcessorID() {
    uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;

    __asm {
        mov eax, 0x8B // Передача команды CPUID
        cpuid

        mov dword ptr[eax], eax // Сохранение результата EAX в переменную
        mov dword ptr[ebx], ebx // Сохранение результата EBX в переменную
        mov dword ptr[ecx], ecx // Сохранение результата ECX в переменную
        mov dword ptr[edx], edx // Сохранение результата EDX в переменную
    }

    return (((uint64_t)ebx << 32) | ((uint64_t)edx));
}

std::string getCPUID() {
    unsigned int regs[4] = { 0 };

    __cpuid((int*)regs, 1);  // Вызов CPUID для Windows

    // Собираем идентификатор процессора на основе значений регистров
    char cpu_id[17]; // 16 символов + завершающий символ '\0'
    snprintf(cpu_id, sizeof(cpu_id), "%08X%08X", regs[3], regs[0]);

    return std::string(cpu_id);
}

std::string chooseDatabaseFile() {
    OPENFILENAME ofn;               // Структура для работы с OpenFileDialog
    wchar_t szFilter[] = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";  // Фильтры файлов
    wchar_t szFile[MAX_PATH] = { 0 };   // Буфер для пути к файлу
    ZeroMemory(&ofn, sizeof(ofn));   // Инициализация структуры
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;            // Окно владельца (NULL для консольного приложения)
    ofn.lpstrFilter = szFilter;      // Применяемый фильтр
    ofn.nFilterIndex = 1;            // Индекс фильтра
    ofn.lpstrFile = szFile;          // Буфер для выбранного файла
    ofn.nMaxFile = MAX_PATH;         // Максимальный размер пути к файлу
    ofn.lpstrTitle = L"Выберите файл базы данных"; // Заголовок диалога
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;  // Флаги диалога

    // Открыть диалог выбора файла
    if (GetOpenFileName(&ofn) != 0) {
        wstring ws(ofn.lpstrFile);
        return wcharToString(ws);
    }
    return std::string();
}

int main() {
    /*Локализатор*/
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Получение хеша процессора
    auto processorID = getCPUID();
    cout << "Hash of the processor: " << processorID << endl;

    auto filepath = chooseDatabaseFile();

    ifstream dbFile(filepath);
    if (!dbFile.is_open()) {
        cerr << "Не удалось открыть выбранный файл базы данных." << endl;
        return 1;
    }

    vector<string> keys;
    string line;
    while (getline(dbFile, line)) {
        keys.push_back(line);
    }
    dbFile.close();

    for (const auto& key : keys) {
        if (processorID == key) {
            cout << "Доступ разрешен!" << endl;
            return 0;
        }
    }

    cout << "Доступ запрещен." << endl;
    return 1;
}
