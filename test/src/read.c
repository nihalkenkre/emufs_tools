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

emufs_table_entry emufs_table[256];

int main(int argc, char **argv)
{
    printf("attempting to read from an emu fs disk\n");

    return 0;
}