#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

uint32_t read_u32(const uint8_t* base, size_t offset)
{
    return *(const uint32_t*)(base + offset);
}

uint32_t find_export_directory(size_t raw, size_t va_off, uint8_t* pe) 
{
    uint32_t name_rva = read_u32(pe, raw + 0x0C);
    uint32_t eat_rva  = read_u32(pe, raw + 0x1C);

    uint32_t edata_va = read_u32(pe, va_off);

    uint32_t name_off = (name_rva - edata_va) + raw;
    char name[260] = {0};

    for (int i = 0; i < 259; i++) 
    {
        if (!pe[name_off + i]) break;
        name[i] = pe[name_off + i];
    }

    printf("[+] Exported symbol: %s\n", name);

    uint32_t eat_off = (eat_rva - edata_va) + raw;
    uint32_t symbol_rva = read_u32(pe, eat_off);

    return symbol_rva;
}


void parse_sections(size_t first, int count, uint8_t* pe) 
{
    for (int i = 0; i < count; i++) 
    {
        size_t off = first + i * 0x28;

        char name[9] = {0};
        memcpy(name, pe + off, 8);

        uint32_t va   = read_u32(pe, off + 0x0C);
        uint32_t raw  = read_u32(pe, off + 0x14);
        uint32_t size = read_u32(pe, off + 0x10);

        printf("[SECTION] %s VA=0x%X RAW=0x%X\n", name, va, raw);

        if (strstr(name, ".edata")) 
        {
            uint32_t rva = find_export_directory(raw, off + 0x0C, pe);
            printf("[+] Export RVA: 0x%X\n", rva);
        }
    }
}

int main(int argc, char** argv) 
{
    if (argc < 2) 
    {
        puts("Usage: pe_parser <file.exe>");
        return 1;
    }

    FILE* f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    uint8_t* pe = malloc(size);
    fread(pe, 1, size, f);
    fclose(f);

    uint32_t e_lfanew = read_u32(pe, 0x3C);
    printf("[+] PE Header @ 0x%X\n", e_lfanew);

    uint16_t sections = pe[e_lfanew + 6] | (pe[e_lfanew + 7] << 8);

    uint16_t opt_size = pe[e_lfanew + 20] | (pe[e_lfanew + 21] << 8);

    size_t first_section = e_lfanew + 24 + opt_size;

    parse_sections(first_section, sections, pe);

    free(pe);
    return 0;
}


/*
    NOTE: bypasses the Windows loader

*/