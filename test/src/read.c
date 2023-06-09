#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct emufs_table_entry
{
    uint8_t file_name[10];
    uint32_t file_offset;
    uint32_t file_size;
} emufs_table_entry;

uint16_t BOOT_SECTOR_SIZE = 512;
uint16_t EMUFS_TABLE_SIZE = 512;

emufs_table_entry *g_table = NULL;

int main(int argc, char **argv)
{
    printf("Attempting to read from an Emu FS disk\n");

    int8_t ret_val = 0;

    if (argc < 2)
    {
        printf("Usage: read <disk_image>.\n");
        ret_val = -1;

        goto shutdown;
    }

    // allocate space for emufs table
    g_table = (emufs_table_entry *)calloc(1, EMUFS_TABLE_SIZE);

    FILE *disk = fopen(argv[1], "rb");

    // look for the end of the boot sector
    fseek(disk, BOOT_SECTOR_SIZE, SEEK_SET);
    fread(g_table, BOOT_SECTOR_SIZE, 1, disk);

    for (uint32_t idx = 0; idx < EMUFS_TABLE_SIZE / sizeof(emufs_table_entry); ++idx)
    {
        if (g_table[idx].file_size == 0)
            break;

        printf("==============\n");

        printf("filename: %s\n", g_table[idx].file_name);
        printf("file offset: %u\n", g_table[idx].file_offset);
        printf("file size: %u\n", g_table[idx].file_size);
    }

shutdown:
    fclose(disk);

    if (g_table)
    {
        free(g_table);
        g_table = NULL;
    }

    return ret_val;
}