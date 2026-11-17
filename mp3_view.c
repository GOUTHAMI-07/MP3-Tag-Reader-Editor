#include<stdio.h>
#include "types.h"
#include "mp3_view.h"
#include<string.h>

Status read_and_validate_view_args(char *argv[], ViewInfo *viewInfo)
{
   if(strstr(argv[2],".mp3")!=NULL){
    char *dot=strstr(argv[2],".mp3");
    if(strlen(dot)==4){
        viewInfo->file_name=argv[2];
        return e_success;
    }
    else{
        printf("Invalide file extension");
        return e_failure;
    }
    }
    else
    {
        printf("Invalid file extension");
        return e_failure;
    }

}

Status open_files(ViewInfo *viewInfo)
{
    viewInfo->fptr_mp3=fopen(viewInfo->file_name,"r");
    if(viewInfo->fptr_mp3==NULL)
    {
        printf("file not found");
        return e_failure;

    }
    return e_success;
    

}

Status view_metadata(ViewInfo *viewInfo)
{
    char header[4];
    if(fread(header,3,1,viewInfo->fptr_mp3)!=1)
    {
        return e_failure;
    }
    header[3]='\0';
    if(strcmp(header,"ID3")!=0)
    {
        printf("header not found\n");
        return e_failure;
    }
    fseek(viewInfo->fptr_mp3, 7 , SEEK_CUR);

    int count=0;
    printf("\n\t\tSONG DETAILS\n");
    printf("--------------------------------------------\n\n");
    while(count<5){
        char frame_id[5];
        unsigned char size[4];
        if(fread(frame_id,4,1,viewInfo->fptr_mp3)!=1)
        {
            return e_failure;

        }
        frame_id[4] = '\0';
        if(fread(size,4,1,viewInfo->fptr_mp3)!=1){
            return e_failure;
        }
        unsigned int frame_size;
        frame_size=(((unsigned int)size[0]<<24)|((unsigned int)size[1]<<16)|
                    ((unsigned int)size[2]<<8)|((unsigned int)size[3]));

        fseek(viewInfo->fptr_mp3,3,SEEK_CUR);
    
    if(frame_size < 1 || frame_size > 10000)
    {
        return e_failure;
    }
    char data[frame_size];

    if(fread(data,frame_size-1,1,viewInfo->fptr_mp3)!=1)
    {
        return e_failure;
    }

    data[frame_size-1]='\0';

    if(strcmp(frame_id,"TIT2")==0)
    {
        printf("Title Name\t:%s\n",data);
        count++;

    }
    else if(strcmp(frame_id,"TPE1")==0)
    {
        printf("Artist Name\t:%s\n",data);
        count++;
    }
    else if(strcmp(frame_id,"TALB")==0)
    {
        printf("Album\t\t:%s\n",data);
        count++;
    }
    else if(strcmp(frame_id,"TYER")==0)
    {
        printf("Year\t\t:%s\n",data);
        count++;
    }
    else if(strcmp(frame_id,"TCON")==0)
    {
        printf("Content Type\t:%s\n",data);
        count++;
    }
    else if(strcmp(frame_id,"COMM")==0)
    {
        printf("Comments\t:%s\n",data);
        count++;
    }
    }
    printf("\n--------------------------------------------\n");
    return e_success;
    

}
