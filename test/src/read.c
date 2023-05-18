#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct emufs_table_entry
{
    uint16_t file_offset;
    uint16_t file_size;
} emufs_table_entry;

uint16_t BOOT_SECTOR_SIZE = 512;
uint16_t EMUFS_TABLE_SIZE = 1024;

emufs_table_entry emufs_table[256];

int main(int argc, char **argv)
{
    printf("attempting to read from an emu fs disk\n");

    if (argc < 2)
    {
        printf("Usage: read <disk_image>.\n");
        return -1;
    }

    FILE *img = fopen(argv[1], "rb");

    if (!img)
    {
        printf("Could not open image %s for reading\n", argv[1]);
        return -2;
    }

    // Read the EMUFS Table Entries
    fseek(img, BOOT_SECTOR_SIZE, SEEK_SET);
    fread(&emufs_table, sizeof(emufs_table_entry), 256, img);

    for (uint16_t idx = 0; idx < 256; ++idx)
    {
        if (emufs_table[idx].file_size == 0)
        {
            continue;
        }

        fseek(img, emufs_table[idx].file_offset, SEEK_SET);
        char name[10];

        fread(name, sizeof(char), 10, img);

        printf("%u: %s\n", idx, name);
    }

    fclose(img);

    return 0;
}