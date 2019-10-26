#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{

    FILE *file;
    char filename[32];
    strcpy(filename, argv[1]);
    strcat(filename, ".txt");

    int N = atoi(argv[1]); //number to count to
    int floor = ((N + 1) / 2);
    int *arr_a = (int *)malloc((N + 1) * sizeof(int)); //+ 1 for 0
    //int arr_a[N];

    arr_a[0] = 0; // for 1
    arr_a[1] = 0; // for 0
    int i;
    for (i = 2; i <= N; i++) //generate all number from 2 to N in array(arr)
    {
        arr_a[i] = i;
    }

    //paralize part below
    for (i = 2; i < (N + 1); i++)
    {
        if (arr_a[i] == 0)
        {
            continue; //loop
        }
        else if (arr_a[i] > floor)
        {
            break; //done with
        }
        int prime = arr_a[i];
        int multiple;
        for (multiple = prime + prime; multiple < (N + 1); multiple += prime) //first one is double
        {
            arr_a[multiple] = 0;
        }
    }
    //till here

    file = fopen(filename, "w"); //write to txt here
    for (i = 0; i < N + 1; i++)
    {
        if (arr_a[i] != 0)
        {
            fprintf(file, "%d ", arr_a[i]);
        }
    }
    fclose(file);

    free(arr_a); //freeing malloc here
    return 0;
}