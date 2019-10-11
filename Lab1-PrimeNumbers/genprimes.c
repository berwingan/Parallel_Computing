#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    //Initialize MPI
    int comm_sz; //number of processes
    int my_rank; //this proceess rank
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int N = atoi(argv[1]); //this might be different
    int floor = ((N + 1) / 2);
    int arr_a[N];
    arr_a[0] = 0; // for 1
    arr_a[1] = 0; // for 0
    int length = sizeof(arr_a) / sizeof(arr_a[0]);
    int i;
    for (i = 2; i <= N; i++) //generate all number from 2 to N in array(arr)
    {
        arr_a[i] = i;
    } //everyone will have their own copy

    //paralize part below
    for (i = 2; i < (N + 1); i++)
    {
        if (arr_a[i] == 0)
        {
            continue;
        }
        else if (arr_a[i] > floor)
        {
            break;
        }
        int prime = arr_a[i];
        int multiple;
        for (multiple = prime + prime; multiple < (N + 1); multiple += prime)
        {
            arr_a[multiple] = 0;
        }
    }
    //till here

    if (my_rank == 0)
    { //only first process writes
        FILE *file;
        char filename[32];
        strcpy(filename, argv[1]);
        strcat(filename, ".txt");
        file = fopen(filename, "w"); //write to txt here
        for (i = 0; i < length + 1; i++)
        {
            if (arr_a[i] != 0)
            {
                fprintf(file, "%d ", arr_a[i]);
            }
        }
        fclose(file);
    }

    MPI_Finalize();
    return 0;
}