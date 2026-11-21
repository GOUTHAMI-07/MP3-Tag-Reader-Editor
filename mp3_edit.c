#include<stdio.h>
#include "types.h"
#include "mp3_edit.h"
#include<string.h>

Status read_and_validate_edit_args(int argc,char *argv[], EditInfo *editInfo){
    if(argc!=5)
    {
        printf("argumment count should be 5\n");
        return e_failure;
    }

    if(strcmp(argv[2],"-t")!=0&&
       strcmp(argv[2],"-a")!=0&&
       strcmp(argv[2],"-A")!=0&&
       strcmp(argv[2],"-y")!=0&&
       strcmp(argv[2],"-c")!=0&&
       strcmp(argv[2],"-m")!=0)
    {
        printf("Invalid option\n");
        return e_failure;
    }
    if(strstr(argv[4],".mp3")!=NULL)
    {
        char *dot=strstr(argv[4],".mp3");
        if(strlen(dot)==4){
            editInfo->file_name=argv[4];
            editInfo->new_data=argv[3];
            editInfo->option=argv[2];
            return e_success;
        }
        else{
            printf("Inavalid file extension\n");
            return e_failure;
        }
    }
    else{
        printf("Inavalid file extension\n");
        return e_failure;
    }
}
Status do_edit(EditInfo *editInfo)
{
    editInfo->fptr_mp3=fopen(editInfo->file_name,"r");
    if(fptr_mp3==NULL){
        printf("Unable to open file\n");
        return e_failure;
    }
    fseek(editInfo->fptr_mp3,10,SEEK_SET);


    char tag[5];
    unsigned int size[4];
    unsigned int farme_size;
    while (fread(tag,1,4,editInfo->fptr_mp3)==4)
    {
        tag[4]='\0';
        if(fread(size,4,1,editInfo->fptr_mp3)!=1){
            return e_failure;
        }
        frame_size=((unsigned int)size[0]<<24)|
                   ((unsigned int)size[1]<<16)|
                   ((unsigned int)size[2]<<8)|
                   (unsigned int)size[3];

        ftell(editInfo->fptr_mp3,3,SEEK_CUR);
    }
}