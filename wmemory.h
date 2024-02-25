#ifndef WMEMORY_H
#define WMEMORY_H

#include "include.h"

typedef signed __int64 sptr64;
typedef signed __int32 sptr32;
typedef signed __int16 sptr16;
typedef signed __int8 sptr8;

typedef unsigned __int64 uptr64;
typedef unsigned __int32 uptr32;
typedef unsigned __int16 uptr16;
typedef unsigned __int8 uptr8;

#define adr64 const unsigned _int64
#define adr32 const unsigned _int32

class Wmemory
{
public:
    Wmemory(const wchar_t* processName) : processId(0), processHandle(NULL)
    {
        HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (processSnapshot != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32W processEntry;
            processEntry.dwSize = sizeof(processEntry);

            if (Process32FirstW(processSnapshot, &processEntry))
            {
                while (true)
                {
                    if (_wcsicmp(processEntry.szExeFile, processName) == 0)
                    {
                        processId = processEntry.th32ProcessID;
                        break;
                    }

                    if (!Process32NextW(processSnapshot, &processEntry))
                        break;
                }
            }
            CloseHandle(processSnapshot);

            if (processId != 0)
                processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        }
    }
    
    uptr64 GetModuleBaseAddress(const wchar_t* moduleName)
    {
        uptr64 moduleBaseAddress = NULL;

        HANDLE moduleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
        if (moduleSnapshot == INVALID_HANDLE_VALUE)
            return 0;

        MODULEENTRY32W moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);

        if (Module32FirstW(moduleSnapshot, &moduleEntry))
        {
            while (true)
            {
                if (_wcsicmp(moduleEntry.szModule, moduleName) == 0)
                {
                    moduleBaseAddress = reinterpret_cast<uptr64>(moduleEntry.modBaseAddr);
                    break;
                }

                if (!Module32NextW(moduleSnapshot, &moduleEntry))
                    break;
            }
        }
        CloseHandle(moduleSnapshot);

        return moduleBaseAddress;
    }

    template <typename T>
    T ReadMemory(uptr64 address)
    {
        T value;
        ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &value, sizeof(T), NULL);

        return value;
    }

    template <typename T>
    void WriteMemory(uptr64 address, const T &value)
    {
        WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), NULL);
    }

private:
    uptr64 processId;

    HANDLE processHandle;
};

#endif