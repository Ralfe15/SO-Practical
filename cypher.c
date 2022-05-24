#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TEXT_SIZE 4000
#define WORD_SIZE 45
#define NUM_CYPHERS 50

struct cypher
{
    char arr[NUM_CYPHERS*2][WORD_SIZE];
    int numWords;
};

void printusages()
{
    fprintf(stderr, "usage: (cypher)\n");
}

struct cypher getCypher()
{
    struct cypher result;
    char word[WORD_SIZE];
    FILE *file;

    file = fopen("cypher.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Cypher file doesn't exits\n");
        return result;
    }

    for (int i = 0; fscanf(file, "%s", result.arr[i]) == 1; result.numWords = ++i);

    fclose(file);
    return result;
}

int main(int argc, char *argv[])
{

    int fd1[2], fd2[2], n = 1;
    pid_t p;
    char string[TEXT_SIZE];
    struct cypher c = getCypher();

    if (strcmp(argv[0], "./cypher"))
    {
        printusages();
        return EXIT_FAILURE;
    }

    while (n < argc)
    {
        if (strcmp(argv[n], "cypher"))
            break;
        n++;
    }

    if (n < argc)
    {
        printusages();
        return EXIT_FAILURE;
    }

    if (c.numWords == 0)
    {
        fprintf(stderr, "There is no cypher file\n");
        return EXIT_FAILURE;
    }

    if (pipe(fd1) == -1)
    {
        fprintf(stderr, "Pipe Failed\n");
        return EXIT_FAILURE;
    }

    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "Pipe Failed\n");
        return EXIT_FAILURE;
    }

    if ((p = fork()) < 0)
    {
        fprintf(stderr, "fork failed\n");
        return EXIT_FAILURE;
    }

    else if (p > 0)
    {
        char received_string[TEXT_SIZE];
        read(STDIN_FILENO, string, TEXT_SIZE);

        close(fd1[0]);
        write(fd1[1], string, strlen(string) + 1);
        close(fd1[1]);
        wait(NULL);

        close(fd2[1]);
        read(fd2[0], received_string, TEXT_SIZE);
        write(STDOUT_FILENO, received_string, strlen(received_string));
        close(fd2[0]);
    }

    else
    {
        char received_string[TEXT_SIZE];
        char sended_string[TEXT_SIZE];
        char word[WORD_SIZE];
        char ch;

        close(fd1[1]);
        read(fd1[0], received_string, TEXT_SIZE);
        close(fd1[0]);

        if (n % 2 != 0)
        {
            for (int i = 0, j = 0, k = 0; (ch = received_string[i]) != '\0'; i++)
            {
                if (!isalpha(ch) && !isdigit(ch))
                {
                    int q;
                    for (q = 0; q < c.numWords; q++)
                    {
                        if (strcmp(word, c.arr[q]) == 0)
                        {
                            if (q % 2 == 0)
                                q++;
                            else
                                q--;
                            break;
                        }
                    }
                    if (q == c.numWords)
                    {
                        for (int l = 0; l < j; l++)
                        {
                            sended_string[k++] = word[l];
                        }
                    }
                    else
                    {
                        for (int l = 0; l < strlen(c.arr[q]); l++)
                        {
                            sended_string[k++] = c.arr[q][l];
                        }
                    }
                    memset(word, 0, WORD_SIZE);
                    j = 0;
                    sended_string[k++] = ch;
                }
                else
                {
                    word[j++] = ch;
                }
            }
        }
        else{
            for(int i = 0; i < strlen(received_string); i++){
                sended_string[i] = received_string[i];
            }
        }

        close(fd2[0]);
        write(fd2[1], sended_string, strlen(sended_string) + 1);
        close(fd2[1]);
        exit(0);
    }

    return (0);
}