#ifndef MP3_VIEW_H
#define MP3_VIEW_H


#include <stdio.h>
#include "types.h"

typedef struct
{
    char *file_name;
    FILE *fptr_mp3;
} ViewInfo;

/* Function declarations */

Status read_and_validate_view_args(int argc,char *argv[], ViewInfo *viewInfo);

Status open_files(ViewInfo *viewInfo);

Status view_metadata(ViewInfo *viewInfo);

void display_data(char *tag, char *data);

void close_files(ViewInfo *viewInfo);

#endif