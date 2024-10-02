#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>

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


int main() {
    const string DB_FILENAME = "keys.txt";
    
    // Получение хеша процессора
    uint64_t processorID = GetProcessorID();
    cout << "Hash of the processor: " << hex << processorID << endl;
    
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
        if (processorID == stoll(key, nullptr, 16)) {
            cout << "Access granted!" << endl;
            return 0;
        }
    }
    
    cout << "Access denied." << endl;
    return 1;
}