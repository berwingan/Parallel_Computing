#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    char filename[32];
    char realname[32];
    char extension[32];
    strcpy(filename, argv[1]);
    strcpy(realname, argv[1]);
    strcpy(extension, argv[2]);
    strcat(realname, "a");
    strcat(filename, extension);
    strcat(realname, extension);

    FILE *fileread;
    FILE *filewrite;
    fileread = fopen(filename, "r");  //read from here
    filewrite = fopen(realname, "w"); //write to txt here

    char ch;
    ch = fgetc(fileread);
    char word[32];
    memset(word, '\0', sizeof(word));
    int i = 0;
    while (ch != EOF)
    {
        while (isspace(ch) == 0)
        {
            word[i] = ch;
            i++;
            ch = fgetc(fileread);
        }
        if (i != 0)
        {
            word[i] = ' ';
            fprintf(filewrite, "%s", word);
            memset(word, '\0', sizeof(word));
            i = 0;
        }
        ch = fgetc(fileread);
    }

    fclose(fileread);
    fclose(filewrite);

    return 0;
}