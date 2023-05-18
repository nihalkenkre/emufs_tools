#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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
    if (argc < 3)
    {
        printf("Usage: copy <disk_image> <file_path>.\n");
        return -1;
    }

    FILE *img = fopen(argv[1], "rb+");
    if (!img)
    {
        printf("Could not image %s for reading.\n", argv[1]);
        return -2;
    }

    fseek(img, BOOT_SECTOR_SIZE, SEEK_SET);
    fread(&emufs_table, sizeof(emufs_table_entry), 256, img);
    if (!img)
    {
        printf("Could not open image %s for writing.\n", argv[1]);
        return -3;
    }

    FILE *file_to_read = fopen(argv[2], "rb");
    if (!file_to_read)
    {
        printf("Could not file %s for reading.\n", argv[2]);
        return -4;
    }

    fseek(file_to_read, 0, SEEK_END);
    uint32_t file_size = ftell(file_to_read);
    rewind(file_to_read);

    uint32_t offset = BOOT_SECTOR_SIZE + EMUFS_TABLE_SIZE;
    uint32_t entry_idx = 0;
    for (entry_idx = 0; entry_idx < 256; ++entry_idx)
    {
        if (emufs_table[entry_idx].file_size == 0)
        {
            break;
        }

        offset += emufs_table[entry_idx].file_size;
    }

    // Write updated table to img
    fseek(img, BOOT_SECTOR_SIZE + (entry_idx * sizeof(emufs_table_entry)), SEEK_SET);
    emufs_table_entry new_entry = {
        .file_offset = offset,
        .file_size = file_size + 10,
    };

    fwrite(&new_entry, sizeof(emufs_table_entry), 1, img);

    // Read the file
    uint8_t *file_data = (uint8_t *)malloc(sizeof(uint8_t) * file_size);

    fread(file_data, file_size, 1, file_to_read);
    fclose(file_to_read);

    // Write the file name at the offset
    char *filename = NULL;
    char* token = strtok(argv[2], "/");

    while (token != NULL)
    {
        filename = token;
        token = strtok(NULL, "/");
    }

    fseek(img, offset, SEEK_SET);
    fwrite(filename, 10, 1, img);

    // Write file data to the image
    fwrite(file_data, file_size, 1, img);

    if (file_data)
    {
        free(file_data);
        file_data = NULL;
    }

    fclose(img);

    return 0;
}