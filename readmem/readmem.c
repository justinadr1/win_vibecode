#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) 
{
    if (argc < 2)
        return 1;

    DWORD pid = (DWORD)strtoul(argv[1], NULL, 10);
    
    HANDLE proc = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);

    if (proc == NULL) 
    {
        printf("Failed to open process. Error: %u\n", GetLastError());
        return 1;
    }

    while (1)
    {
        DWORD_PTR address;
        printf("Enter address: ");
        scanf("%p", (void**)&address);

        BYTE buffer[16];
        SIZE_T bytesRead;

        if (ReadProcessMemory(proc, (LPCVOID)address, buffer, 16, &bytesRead)) 
        {
            printf("16 Bytes read: ");
            for (int i = 0; i < 16; i++)
            {
                printf("%02X ", buffer[i]);
            }
        } 
        else 
        {
            printf("ReadProcessMemory failed. Error: %u\n", GetLastError());
        }
        printf("\n");
    }

    CloseHandle(proc);
    return 0;
}
