#ifndef MP3_READER_H
#define MP3_READER_H

void view_mp3_tags(const char *file_name);
void edit_mp3_tag(char *filename,char *tag_id,char *new_data);
void help();

#endif