#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1024

void printbuffer(char buf[], int j)
{
    if (buf[0] != ' ')
        printf("[%d] %s\n", j, buf);
    else
        printf("[%d]%s\n", j, buf);
    memset(buf, 0, strlen(buf));
}

void printusages(){
    printf("usage: phrases [-l] file\n");
}

int main(int argc, char *argv[])
{
    // open the file
    FILE *fp;
    switch(argc){
        case 1: printusages(); return EXIT_FAILURE;
        case 2: fp = fopen(argv[1], "r"); break;
        case 3: fp = fopen(argv[2], "r"); break;
        default: printusages(); return EXIT_FAILURE;
    }
    int i = 0;
    int j = 1;
    char buf[MAX_LEN];
    // Return if could not open file
    if (fp == NULL)
        return 0;
    do
    {
        char c = fgetc(fp);
        if (c == '.' || c == '!' || c == '?')
        {
            buf[i] = c;
            if (!strcmp(argv[1], "-l"))
                printbuffer(buf, j);

            i = 0;
            j++;
        }
        else
        {
            buf[i] = c;
            i++;
        }
    } while (!feof(fp));
    buf[(strlen(buf) - 1)] = '\0';
    if (!strcmp(argv[1], "-l")){
        printbuffer(buf, j);
    }
    else{
        printf("%d\n", j);
    }
    
    fclose(fp);
    return (0);
}