#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    char *flag = argv[1];
    srand(1336);
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        printf("%i, ", rand() % 3);
    }
}