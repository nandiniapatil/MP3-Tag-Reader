#include <stdio.h>
#include <string.h>

void view_mp3_tags(const char *file_name) 
{
    FILE *fptr = fopen(file_name, "rb");
    if (fptr == NULL) 
    {
        printf("Error: Can't find the file.\n");
        return;
    }

    // Skip the ID3 Header (10 bytes)
    fseek(fptr, 10, SEEK_SET);

    printf("\n--- MP3 INFORMATION ---\n");

    for (int i = 0; i < 6; i++) 
    {
        char tag_id[5] = {0};   
        int size = 0;           
        char data[100] = {0};   

        // 1. Read Tag Name
        if (fread(tag_id, 1, 4, fptr) < 4) break; 

        // 2. Read Tag Size
        fread(&size, 1, 4, fptr);

        // 3. Swap Bytes (Big Endian to Little Endian)
        char *p = (char *)&size;
        char temp;
        temp = p[0]; p[0] = p[3]; p[3] = temp;
        temp = p[1]; p[1] = p[2]; p[2] = temp;

        // 4. Skip 3 bytes (Flags + Encoding)
        fseek(fptr, 3, SEEK_CUR);

        // 5. Read the actual text
        // Ensure we don't overflow our 'data' array (max 99 chars)
        int read_len = (size - 1 < 99) ? (size - 1) : 99;
        fread(data, 1, read_len, fptr);
        data[read_len] = '\0'; // Safety Null Terminator

        // 6. Print with readable labels
        if      (strcmp(tag_id, "TIT2") == 0) printf("Title   : %s\n", data);
        else if (strcmp(tag_id, "TPE1") == 0) printf("Artist  : %s\n", data);
        else if (strcmp(tag_id, "TALB") == 0) printf("Album   : %s\n", data);
        else if (strcmp(tag_id, "TYER") == 0) printf("Year    : %s\n", data);
        else if (strcmp(tag_id, "TCON") == 0) printf("Genre   : %s\n", data);
        else if (strcmp(tag_id, "COMM") == 0) printf("Comment : %s\n", data);
        else printf("%s    : %s\n", tag_id, data);
    }

    fclose(fptr);
    printf("-----------------------\n");
}