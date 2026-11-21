#ifndef MP3_EDIT_H
#define MP3_EDIT_H

#include<stdio.h>
#include "types.h"

typedef struct
{
    char *file_name;
    char *new_data;
    char *option;
    FILE *fptr_mp3;

} EditInfo;

Status read_and_validate_edit_args(int argc,char *argv[], EditInfo *editInfo);

Status do_edit(EditInfo *editInfo);

Status edit_tag(EditInfo *editInfo);

#endif