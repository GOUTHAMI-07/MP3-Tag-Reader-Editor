#include <stdio.h>
#include "mp3_view.h"
#include "types.h"
#include <string.h>
#include "common.h"

OperationType check_operation_type(char opt);

int main(int argc,char *argv[])
{
    ViewInfo viewInfo;
    if(argc<3)
    {
        printf("Invalid argument\n");
        return 1;
    }
    if(check_operation_type(argv[1][1])==e_view)
    {
        if(read_and_validate_view_args(argv,&viewInfo)==e_success)
        {
            if(open_files(&viewInfo)==e_success)
            {
                if(view_metadata(&viewInfo)==e_success)
                {
                    printf("\t\tEND VIEW\n");
                    fclose(viewInfo.fptr_mp3);

                }
                else
                {
                    printf("Error: Unable to view metadata\n");
                }

            }
            else
            {
                printf("Error: Unable to open file\n");
            }
        }
        else
        {
            printf("Error:Invalid arguments\n");
        }
    }
    
}

OperationType check_operation_type(char opt)
{
    if(opt=='v')
    {
        return e_view;
    }
    else if(opt=='e'){
        return e_edit;
    }
    else{
        return e_unsupported;
    }
}
