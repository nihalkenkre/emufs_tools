#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct emufs_table_entry
{
    uint8_t file_name[10];
    uint32_t file_offset;
    uint32_t file_size;
} __attribute__((packed)) emufs_table_entry;

uint16_t BOOT_SECTOR_SIZE = 512;
uint16_t EMUFS_TABLE_SIZE = 512;

emufs_table_entry *g_table = NULL;
int main(int argc, char **argv)
{
    int8_t retval = 0;

    if (argc < 2)
    {
        printf("Usage: emufs_dir <disk_name>.\n");
        retval = -1;

        goto shutdown;
    }

    g_table = (emufs_table_entry *)calloc(1, EMUFS_TABLE_SIZE);

    FILE *img = fopen(argv[1], "rb");

    if (!img)
    {
        printf("Could not open disk image for reading.\n");
        retval = -2;

        goto shutdown;
    }

    fseek(img, BOOT_SECTOR_SIZE, SEEK_SET);
    fread(g_table, EMUFS_TABLE_SIZE, 1, img);

    for (uint32_t idx = 0; idx < EMUFS_TABLE_SIZE / sizeof(emufs_table_entry); ++idx)
    {
        if (g_table[idx].file_size == 0)
            break;

        printf("==============\n");

        printf("filename: %s\n", g_table[idx].file_name);
        printf("file offset: %u\n", g_table[idx].file_offset);
        printf("file size: %u\n", g_table[idx].file_size);
    }

    printf("End of table entries\n");

shutdown:
    if (img)
    {
        fclose(img);
    }

    if (g_table != NULL)
    {
        free(g_table);
        g_table = NULL;
    }

    return retval;
}