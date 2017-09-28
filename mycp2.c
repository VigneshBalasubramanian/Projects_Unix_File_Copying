// CS590_File_Copy_Assignment_1_Vignesh_Balasubramanian//
// Vignesh Balasubramanian//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{

//Variable declaration//
int File_Descriptor_In,File_Descriptor_Out;

//For copying one file to another//

if(argc==3)
{
        char *Source_File = argv[1];
        char *Destination_File = argv[2];
        struct stat check;
        int i;

        //Checking if source and destination files are same//

        if(strcmp(argv[1],argv[2])==0)
        {
                printf("\nSource and Destination Files can't be the same\n");
		exit(0);
        }
		
	//Checking for Permission Access of destination file//
		                 
		access(Destination_File,W_OK);
        
		if(EACCES == errno)
		
		{
                          printf("\nPermission not granted\n");
                          exit(0);
     		}
        else
        {
        int result = access (Destination_File, F_OK);

	//Checking for existence of destination file if not, creating one//

        if(result<0)
        {

                printf("\nThis destination file doesn't exist. Creating one... \n");
        }

        lstat(argv[2], &check);

	//Checking for existence of source file//


        printf("Source File: %s\n",Source_File);
        printf("Destination File/Directory: %s\n",Destination_File);
        if(( File_Descriptor_In = open(Source_File, O_RDONLY,0777))<0)
        {
                perror("\n The source file doesn't exist.\n");
                printf("\n The non-exisitng file's name: %s\n",Source_File);
        }
        else
        {
                printf("\nCopying file...\n");

		//Confirming with user if the existing file has to be replaced//
		
                if((File_Descriptor_Out = open(Destination_File,O_WRONLY))>0)
                {
                        char user_choice;
                        printf("\nFile already exists. Continue overriding? Y or N?\n");
                        scanf("%s",&user_choice);
                        if(user_choice=='N')
                        {
                                return 0;
                        }
                        else if(user_choice=='Y')
                        {
				open(Destination_File,O_TRUNC);
                                char buf[8192];
                                size_t result = read(File_Descriptor_In, &buf[0], sizeof(buf));
                                write(File_Descriptor_Out, &buf[0], result) == result;
                                printf("\nCopied successfully\n");
                                return 0;
                        }
                }
                else
                {
                        if (S_ISDIR(check.st_mode))
                        {

                                strcat(Destination_File, "/");
                                strcat(Destination_File, Source_File);
                                if((File_Descriptor_Out=open(Destination_File,O_WRONLY,0777))>0)
                                {
                                        char user_choice;
                                        printf("\nFile already exists. Continue overriding? Y or N?\n");
                                        scanf("%s",&user_choice);
                                        if(user_choice=='N')
                                        {
                                                return 0;
                                        }
                                        else if(user_choice=='Y')
                                        {
						open(Destination_File,O_TRUNC);
                                                char buf[8192];
                                                size_t result = read(File_Descriptor_In, &buf[0], sizeof(buf));
                                                write(File_Descriptor_Out, &buf[0], result) == result;
                                                printf("\nCopied successfully\n");
                                                return 0;
                                        }

                                }
                                else
                                {
                                        File_Descriptor_Out =open(Destination_File,O_CREAT|O_WRONLY,0777);
                                        char buf[8192];
                                        size_t result = read(File_Descriptor_In, &buf[0], sizeof(buf));
                                        write(File_Descriptor_Out, &buf[0], result) == result;
                                        printf("\nCopied successfully\n");
                                        return 0;
                                }
                        }
                        else
                        {
                                File_Descriptor_Out=open(Destination_File,O_CREAT|O_WRONLY,0777);
                                char buf[8192];
                                size_t result = read(File_Descriptor_In, &buf[0], sizeof(buf));
                                write(File_Descriptor_Out, &buf[0], result) == result;
                                printf("\nCopied successfully\n");
                                return 0;
                        }
                 }
           }
      }
}

//For copying files into directory//

else if(argc>3)
{

//Variable declaration//

struct stat check;
int i,count=argc-1;
char *Destination_File1;
char *Destination_File2 = argv[count];

lstat(Destination_File2, &check);
Destination_File1=malloc(strlen(argv[argc-1])+1);
strcat(Destination_File1,argv[count]);


        i=1;

        while(i<count)
        {
        printf("Source File %d:%s\n",i,argv[i]);
        i=i+1;
        }

        printf("Destination Directory: %s\n",argv[count]);

	//Checking for permission access of directory//
		access(Destination_File2,W_OK|X_OK);
		if(EACCES == errno)
		
		{
                          printf("\nPermission not granted\n");
                          exit(0);
        }

	//Checking for the existence of the destination directory if not, creating one//	

        int result = access (argv[count], F_OK);
        if(result<0)
                {
                        printf("\nThis destination directory %s doesn't exist. Creating one... \n",argv[count]);
                        mkdir(argv[count],0777);
                 }

	//Starting the copying files operation one by one//

	for(i=count-1;i>=1;i--)
        {

		//Checking for the existence of the input file//

                if(( File_Descriptor_In = open(argv[i], O_RDONLY))<0)
                {
                        perror("\n The input file doesn't exist. \n");
                        printf("\nThe file that doesn't exist is: %s \n", argv[i]);
                        continue;
                }
		//Checking for the access of the destination directory//
                else
                {
                access(Destination_File2,W_OK|X_OK);
                  if(EACCES == errno)
             {
		printf("\nPermission not granted\n");
		exit(0);
	     }

             free(Destination_File1);
             Destination_File1=malloc(strlen(argv[i])+strlen(argv[argc-1])+2);
             strcat(Destination_File1,argv[count]);
             strcat(Destination_File1, "/");
             strcat(Destination_File1, argv[i]);
 
            if((File_Descriptor_Out=open(Destination_File1,O_WRONLY))>0)
 	            {
                    char user_choice;
                    printf("\nFile(s) already exists. Continue overriding? Y or N?\n");
                    scanf("%s",&user_choice);
                                   if(user_choice=='N')
                                   {
	                           printf("\nPlease check the file list and try again\n");
                                   }
                                   else if(user_choice=='Y')
                                   {
                                   char buf[8192];
                                   size_t result = read(File_Descriptor_In, &buf[0], sizeof(buf));
                                   write(File_Descriptor_Out, &buf[0], result);
                                   printf("\nCopied successfully\n");
                                   }                                           
                     }
            else
                     {                                                   
                     File_Descriptor_Out=open(Destination_File1,O_CREAT|O_WRONLY,0777);
                     char buf[8192];
                     size_t result = read(File_Descriptor_In, &buf[0], sizeof(buf));
                     write(File_Descriptor_Out, &buf[0], result);
                     printf("\nCopied successfully\n");
                     }
               }
       }
}

	//Displaying invalidity of source and destination files//

        else
        {
                printf("\nInvalid source and destination\n");
                exit(1);
        }
}

