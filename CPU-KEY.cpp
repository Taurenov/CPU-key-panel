#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>
#if defined(_WIN32) || defined(_WIN64)
#include <intrin.h>  // Для Windows
#else
#include <cpuid.h>   // Для Linux и MacOS
#endif


using namespace std;
const string DB_FILENAME = "keys.txt";

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

#if defined(_WIN32) || defined(_WIN64)
    __cpuid((int*)regs, 1);  // Вызов CPUID для Windows
#else
    __get_cpuid(1, &regs[0], &regs[1], &regs[2], &regs[3]);  // Вызов CPUID для Linux и MacOS
#endif

    // Собираем идентификатор процессора на основе значений регистров
    char cpu_id[17]; // 16 символов + завершающий символ '\0'
    snprintf(cpu_id, sizeof(cpu_id), "%08X%08X", regs[3], regs[0]);

    return std::string(cpu_id);
}

int main() {

    // Получение хеша процессора
    auto processorID = getCPUID();
    cout << "Hash of the processor: "<< processorID << endl;

    ifstream dbFile(DB_FILENAME);
    if (!dbFile.is_open()) {
        cerr << "Failed to open database file." << endl;
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
            cout << "Access granted!" << endl;
            return 0;
        }
    }

    cout << "Access denied." << endl;
    return 1;
}
