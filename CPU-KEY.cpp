#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <shlobj.h> // Для OpenFileDialog

using namespace std;

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

void chooseDatabaseFile() {
    OPENFILENAME ofn;       // Структура для работы с OpenFileDialog
    TCHAR szFilter[] = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = NULL;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = _T("Выберите файл базы данных");
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    if (GetOpenFileName(&ofn) != 0) {
        string filename = ofn.lpstrFile;
        cout << "Выбранный файл базы данных: " << filename << endl;
    } else {
        cout << "Отмена выбора файла базы данных." << endl;
    }
}

int main() {

    // Получение хеша процессора
    auto processorID = getCPUID();
    cout << "Hash of the processor: " << processorID << endl;

    chooseDatabaseFile();

    ifstream dbFile;
    dbFile.open(filename);
    if (!dbFile.good()) {
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
