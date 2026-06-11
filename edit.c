#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simple helper to swap bytes (Big Endian <-> Little Endian)
int swap_endian(int val) {
    char *p = (char *)&val;
    char temp;
    temp = p[0]; p[0] = p[3]; p[3] = temp;
    temp = p[1]; p[1] = p[2]; p[2] = temp;
    return val;
}

void edit_mp3_tag(char *file_name, char *tag_to_edit, char *new_content) {
    FILE *fptr = fopen(file_name, "rb");
    FILE *temp = fopen("temp.mp3", "wb");

    if (!fptr || !temp) {
        printf("Error opening files!\n");
        return;
    }

    // 1. Copy the 10-byte ID3 Header
    char header[10];
    fread(header, 1, 10, fptr);
    fwrite(header, 1, 10, temp);

    // 2. Process the 6 main tags
    for (int i = 0; i < 6; i++) {
        char tag_id[5] = {0};
        int size = 0;
        char flags[3] = {0};

        fread(tag_id, 1, 4, fptr);   // Read Tag ID
        fread(&size, 4, 1, fptr);    // Read Size
        fread(flags, 1, 2, fptr);    // Read Flags (2 bytes)

        int old_size = swap_endian(size);

        // If this is the tag we want to change
        if (strcmp(tag_id, tag_to_edit) == 0) {
            // Write the same Tag ID
            fwrite(tag_id, 1, 4, temp);

            // Write the NEW Size (length of new string + 1 for encoding byte)
            int new_size_val = strlen(new_content) + 1;
            int big_endian_size = swap_endian(new_size_val);
            fwrite(&big_endian_size, 4, 1, temp);

            // Write the same Flags
            fwrite(flags, 1, 2, temp);

            // Write 1 byte for Encoding (usually 0) and the New Content
            fputc(0, temp); 
            fwrite(new_content, 1, strlen(new_content), temp);

            // Skip the OLD data in the original file
            fseek(fptr, old_size, SEEK_CUR);
        } 
        else {
            // Just copy the existing tag as it is
            fwrite(tag_id, 1, 4, temp);
            fwrite(&size, 4, 1, temp);
            fwrite(flags, 1, 2, temp);

            char *buffer = malloc(old_size);
            fread(buffer, 1, old_size, fptr);
            fwrite(buffer, 1, old_size, temp);
            free(buffer);
        }
    }

    // 3. Copy everything else (the actual music data)
    char ch;
    while (fread(&ch, 1, 1, fptr) > 0) {
        fwrite(&ch, 1, 1, temp);
    }

    fclose(fptr);
    fclose(temp);

    // 4. Swap the files
    remove(file_name);
    rename("temp.mp3", file_name);

    printf("Tag '%s' updated successfully to: %s\n", tag_to_edit, new_content);
}