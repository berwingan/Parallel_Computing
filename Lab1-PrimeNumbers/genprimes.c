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

    int *arr_a = (int *)malloc((N + 1) * sizeof(int)); //dynamically allocating array of ints
    int *arr_b = (int *)malloc((N + 1) * sizeof(int)); //absoulte need two arrays
    arr_a[0] = 0;                                      // for 1
    arr_a[1] = 0;                                      // for 0

    int i;
    for (i = 2; i <= N; i++) //generate all number from 2 to N in array(arr)
    {
        arr_a[i] = i;
    } //everyone will have their own copy
    //print out rank and size here
    //printf("I am process %d out of %d\n", my_rank, comm_sz);
    //paralize part below
    //split parts for two
    //---------------------------- this part for even
    if (my_rank == (comm_sz - 1)) //currently even is being done by the last core
    {                             //last processor
        int two;
        for (two = 2 + 2; two < (N + 1); two += 2)
        {
            arr_a[two] = 0; //takes out all the even
        }
    }
    //test, parallel when done with update
    //------------------------------this part for even

    // int update, update_odd, update_even;
    // update_odd = (((comm_sz - my_rank) - 1) * 2) + 1;
    // update_even = (my_rank * 2) + 1;
    int update = comm_sz * 2;
    for (i = ((my_rank * 2) + 3); i < (N + 1); i += update) //check time
    {
        if (arr_a[i] == 0)
        {
            continue;
        } //take away ?
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
        // update = update_odd;
        // update_odd = update_even;
        // update_even = update;
    }
    //till here
    //reduce here with MIN

    MPI_Reduce(arr_a, arr_b, (N + 1), MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD); //needs to be broken up

    if (my_rank == 0)
    { //only first process writes
        FILE *file;
        char filename[32];
        strcpy(filename, argv[1]);
        strcat(filename, ".txt");
        file = fopen(filename, "w"); //write to txt here
        for (i = 0; i < (N + 1); i++)
        {
            if (arr_b[i] != 0)
            {
                fprintf(file, "%d ", arr_b[i]);
            }
        }
        fclose(file);
    }

    free(arr_b);
    free(arr_a); //freeing malloc here
    MPI_Finalize();
    return 0;
}