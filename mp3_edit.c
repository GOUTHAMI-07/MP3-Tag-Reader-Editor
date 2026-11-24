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
    if(editInfo->fptr_mp3==NULL){
        printf("Unable to open file\n");
        return e_failure;
    }
    fseek(editInfo->fptr_mp3,10,SEEK_SET);

    char *frame_id=NULL;
        if(strcmp(editInfo->option,"-t")==0)
        {
            frame_id="TIT2";
        }
        else if(strcmp(editInfo->option,"-a")==0)
        {
            frame_id="TPE1";
        }
        else if(strcmp(editInfo->option,"-A")==0)
        {
            frame_id="TALB";
        }
        else if(strcmp(editInfo->option,"-y")==0)
        {
            frame_id="TYER";
        }
        else if(strcmp(editInfo->option,"-c")==0)
        {
            frame_id="COMM";
        }
        else if(strcmp(editInfo->option,"-m")==0)
        {
            frame_id="TCON";
        }
        if(frame_id==NULL){
            fclose(editInfo->fptr_mp3);
            return e_failure;
        }

    char tag[5];
    unsigned char size[4];
    unsigned int frame_size;

    FILE *fptr_temp=fopen("temp.mp3","w");
    if(fptr_temp==NULL)
    {
        fclose(editInfo->fptr_mp3);
        return e_failure;
    }

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

        fseek(editInfo->fptr_mp3,3,SEEK_CUR);
        long pos=ftell(editInfo->fptr_mp3);
        if(strcmp(tag,frame_id)==0)
        {
            unsigned char header[10];
            rewind(editInfo->fptr_mp3);
            if(fread(header,10,1,editInfo->fptr_mp3)!=1){
                fclose(editInfo->fptr_mp3);
                fclose(fptr_temp);
                return e_failure;
            }
            fwrite(header,10,1,fptr_temp);

            fseek(editInfo->fptr_mp3,pos,SEEK_SET);

            unsigned int new_size=strlen(editInfo->new_data)+1;
            unsigned char new_size_bytes[4];
            new_size_bytes[0] = (new_size >> 24) & 0xFF;
            new_size_bytes[1] = (new_size >> 16) & 0xFF;
            new_size_bytes[2] = (new_size >> 8) & 0xFF;
            new_size_bytes[3] = new_size & 0xFF;

            fwrite(tag,4,1,fptr_temp);
            fwrite(new_size_bytes,4,1,fptr_temp);

            unsigned char flag[2];
            fseek(editInfo->fptr_mp3,-3,SEEK_CUR);
            if(fread(flag,2,1,editInfo->fptr_mp3)!=1){
                fclose(editInfo->fptr_mp3);
                fclose(fptr_temp);
                return e_failure;
            }
            fwrite(flag,2,1,fptr_temp);

            unsigned char encoding=0;
            fwrite(&encoding,1,1,fptr_temp);

            fwrite(editInfo->new_data,strlen(editInfo->new_data),1,fptr_temp);

            fseek(editInfo->fptr_mp3,frame_size,SEEK_CUR);
            char ch;
            while(fread(&ch,1,1,editInfo->fptr_mp3)==1)
            {
                fwrite(&ch,1,1,fptr_temp);
            }

            break;
        }
        else
        {
            fwrite(tag,1,4,fptr_temp);
            fwrite(size,1,4,fptr_temp);
            unsigned char flags[2];
            fseek(editInfo->fptr_mp3,-3,SEEK_CUR);
            fread(flags,1,2,editInfo->fptr_mp3);
            fwrite(flags,1,2,fptr_temp);
            char null;
            fread(&null,1,1,editInfo->fptr_mp3);
            fwrite(&null,1,1,fptr_temp);
            char data[frame_size-1];
            fread(data,1,frame_size-1,editInfo->fptr_mp3);
            fwrite(data,1,frame_size-1,fptr_temp);
        }
    }

fclose(editInfo->fptr_mp3);
fclose(fptr_temp);
if(rename("temp.mp3",editInfo->file_name) != 0)
{
    printf("Rename failed\n");
    return e_failure;
}
return e_success;

}