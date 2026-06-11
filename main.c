/*---------------------------------------------------------------------
|                                                                     |
|  Name : Shrish Rahav G                                              |
|  Date : 10/03/2026                                                  |
|  Batch Id : 25040_004                                               |
|  project : Mp3 tag reader                                           |
|                                                                     |
|-----------------------------------------------------------------------

*/

//new
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mp3.h"

void help()
{
    printf("\n************************** Help Menu ***************************\n");
    printf("1. -v : View mp3 contents\n");
    printf("Usage for View:\n");
    printf("    ./a.out -v <file.mp3>\n\n");
    printf("2. -e : Edit mp3 contents\n");
    printf("Usage for Edit:\n");
    printf("   ./a.out -e <file.mp3> -t <New_title> \n");
    printf("   ./a.out -e <file.mp3> -A <New_artist> \n");
    printf("   ./a.out -e <file.mp3> -a <New_album>  \n");
    printf("   ./a.out -e <file.mp3> -y <New_year>    \n");
    printf("   ./a.out -e <file.mp3> -C <New_Composer> \n");
    printf("   ./a.out -e <file.mp3> -c <New_Content> \n");
    printf("****************************************************************\n\n");
}

int main(int argc, char *argv[])
{
    // 1. Check for Help
    if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0))
    {
        help();
        return 0;
    }

    // 2. Handle View Mode: ./a.out -v song.mp3
    if (argc == 3 && strcmp(argv[1], "-v") == 0)
    {
        if (strstr(argv[2], ".mp3") == NULL)
        {
            printf("Error: File is not an .mp3 file.\n");
            return 1;
        }
        view_mp3_tags(argv[2]);
    }

    // 3. Handle Edit Mode: ./a.out -e song.mp3 -t New Title
    else if (argc >= 5 && strcmp(argv[1], "-e") == 0)
    {
        char *tag = NULL;
        char new_content[100] = ""; // INITIALIZED to empty string to avoid garbage

        // Check if file is valid
        if (strstr(argv[2], ".mp3") == NULL)
        {
            printf("Error: File is not an .mp3 file.\n");
            return 1;
        }

        // Map short flags to ID3v2 Tag Names
        if      (strcmp(argv[3], "-t") == 0) tag = "TIT2";
        else if (strcmp(argv[3], "-A") == 0) tag = "TPE1";
        else if (strcmp(argv[3], "-a") == 0) tag = "TALB";
        else if (strcmp(argv[3], "-y") == 0) tag = "TYER";
        else if (strcmp(argv[3], "-C") == 0) tag = "COMM";
        else if (strcmp(argv[3], "-c") == 0) tag = "TCON";

        if (tag != NULL)
        {
            // Collect all words after the flag into one string
            for (int i = 4; i < argc; i++)
            {
                strcat(new_content, argv[i]);
                if (i < argc - 1) strcat(new_content, " ");
            }
            edit_mp3_tag(argv[2], tag, new_content);
        }
        else
        {
            printf("Invalid edit option '%s'. Use -h for help.\n", argv[3]);
        }
    }

    // 4. Handle Errors
    else
    {
        printf("Invalid Arguments! Use ./a.out -h for instructions.\n");
    }

    return 0;
}