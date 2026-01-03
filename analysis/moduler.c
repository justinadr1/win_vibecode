#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG(level, msg) do {                                  \
    const char* color = "\x1b[0m";                            \
    const char* prefix = "";                                  \
    if (strcmp(level, "DEBUG") == 0) {                         \
        color = "\x1b[94m"; prefix = "[||]";                  \
    } else if (strcmp(level, "INFO") == 0) {                   \
        color = "\x1b[32m"; prefix = "[+]";                   \
    } else if (strcmp(level, "WARN") == 0) {                   \
        color = "\x1b[33m"; prefix = "[!]";                   \
    } else if (strcmp(level, "ERROR") == 0) {                  \
        color = "\x1b[31m"; prefix = "[-]";                   \
    }                                                          \
    printf("%s%s %s\x1b[0m\n", color, prefix, msg);            \
} while (0)

DWORD get_pid(const char* process_name)
{
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        LOG("ERROR", "Failed to snapshot processes");
        exit(1);
    }

    if (!Process32First(snap, &pe)) {
        CloseHandle(snap);
        LOG("ERROR", "Process32First failed");
        exit(1);
    }

    DWORD pid = 0;
    do {
        if (_stricmp(pe.szExeFile, process_name) == 0) 
        {
            pid = pe.th32ProcessID;
            break;
        }
    } while (Process32Next(snap, &pe));

    CloseHandle(snap);

    if (pid == 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Unable to get PID for %s : PROCESS DOESN'T EXIST", process_name);
        LOG("ERROR", buf);
        exit(1);
    }

    return pid;
}

int main(int argc, char* argv[])
{
    if (argc != 2) 
    {
        LOG("ERROR", "Usage: importer.exe <PROCESS NAME>");
        return 1;
    }

    DWORD pid = get_pid(argv[1]);

    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!process) 
    {
        LOG("ERROR", "Failed to open process");
        return 1;
    }

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

    if (snapshot == INVALID_HANDLE_VALUE) 
    {
        LOG("ERROR", "Failed to create module snapshot");
        CloseHandle(process);
        return 1;
    }

    MODULEENTRY32 me;
    me.dwSize = sizeof(MODULEENTRY32);

    if (!Module32First(snapshot, &me)) 
    {
        LOG("ERROR", "Module32First failed");
        CloseHandle(snapshot);
        CloseHandle(process);
        return 1;
    }

    printf("%-34s | %-20s\n", "Module Name", "Base Address");
    printf("--------------------------------------------------\n");

    do {
        char buffer[512];
        snprintf(buffer, sizeof(buffer), "%-30s | 0x%p", me.szModule, me.modBaseAddr);
        LOG("INFO", buffer);
    } while (Module32Next(snapshot, &me));

    CloseHandle(snapshot);
    CloseHandle(process);
}
