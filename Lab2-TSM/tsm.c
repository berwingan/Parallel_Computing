#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <omp.h>
#include <limits.h>

int main(int argc, char *argv[])
{
	int thread = atoi(argv[1]);
	omp_set_num_threads(thread); //just setting nothing else

	int cities = atoi(argv[2]);
	int *ptr, **arr;
	int i, j, len = 0;
	len = sizeof(int *) * (cities) + sizeof(int) * cities * cities;
	arr = (int **)malloc(len);   //a citties by citties 2D array
	ptr = (int *)(arr + cities); //ptr pointing to first ellemt
	//for pointers to point row pointer
	for (i = 0; i < cities; i++)
	{
		arr[i] = (ptr + cities * i);
	}
	//------------------------------
	int ok = cities;
	ok--; //casue we start from 0
	unsigned long long r = 1, il = 0;
	while (ok != 0)
	{
		r = r * ok;
		ok--;
	}
	unsigned long long c = cities, lent = 0;
	unsigned long long *ptr1, **arr1;
	lent = sizeof(unsigned long long *) * r + sizeof(unsigned long long) * c * r;
	arr1 = (unsigned long long **)malloc(lent);
	ptr1 = (unsigned long long *)(arr1 + r); //this is apparently fine
	for (il = 0; il < r; il++)
	{
		arr1[il] = (ptr1 + c * il);
	} //this is still apparently fine up to a10
	//--------------------------------
	FILE *fp;
	fp = fopen(argv[3], "r");
	if (fp == NULL)
	{
		printf("That was not a real test file\n");
		return 0;
	}
	for (i = 0; i < (cities); i++)
	{
		for (j = 0; j < cities; j++)
		{
			fscanf(fp, "%d", &arr[i][j]);
		}
	} //writes from file to malloc in 2D array
	//malloc an array
	int *best_order = malloc(cities * sizeof(int)); //this have to be public to share between threads
	for (i = 0; i < cities; i++)
	{
		best_order[i] = i;
	}

	unsigned long long counter = 0;
	int temp;
	//printf("rows are %lld\n",r);

	while (1)
	{ //to be placed in arr1[counter][i];
		for (i = 0; i < cities; i++)
		{
			arr1[counter][i] = best_order[i];
			//printf("%d->",best_order[i]);
		}
		//printf("[]--%lld\n",counter);
		counter++;
		int x = -1;
		int x_val = INT_MIN;
		for (i = cities - 2; i > 0; i--)
		{
			if ((best_order[i] < best_order[i + 1]) && (best_order[i] > x))
			{
				x_val = best_order[i];
				x = i; //use this to swap
				break;
			}
		}
		if (x == -1)
		{
			break;
		} //this means that there is something bigger
		//-------------
		//printf("x is %d with a value of %d\n",x,x_val);
		int small_y = 0;
		int y = -1;
		for (i = 0; i < cities; i++)
		{
			if (best_order[i] > x_val)
			{
				y = i;
				small_y = best_order[i];
			}
		}
		if (y == -1)
		{
			printf("here\n");
			break;
		}
		//---------------------------
		//printf("Swapping %d with %d\n",small_y,x_val);
		best_order[x] = small_y;
		best_order[y] = x_val; //swap

		i = x + 1;
		j = cities - 1;
		while (i < j)
		{
			temp = best_order[i];
			best_order[i] = best_order[j];
			best_order[j] = temp;
			i++;
			j--;
		}
	}

	int best_distance = INT_MAX;
	unsigned long long best_ord[cities];
	unsigned long long agg2[thread]; //shared by everyone
#pragma omp parallel for private(best_ord, i) reduction(min \
														: best_distance) //<--based on reduction(best_ord) //gotta private the i
	for (il = 0; il < r; il++)
	{
		//printf("%lld",il);
		int internal_distance = 0;
		for (i = 0; i < cities - 1; i++)
		{															//-1 cause it can't go all the way to the end
			internal_distance += arr[arr1[il][i]][arr1[il][i + 1]]; //adds up distance
																	//printf("%lld to %lld //",arr1[il][i],arr1[il][i+1]);
																	//printf("%d-->",arr[arr1[il][i]][arr1[il][i+1]]);
		}
		//printf(" with distance of %d\n",internal_distance);
		//printf("----%lld\n",il);

		if (internal_distance < best_distance)
		{									   // && internal_distance!=0){
			best_distance = internal_distance; //somehow save the il info, tie it with best_distance
			//memcpy(best_ord,arr1[il],sizeof(unsigned long long) * cities); //lastprivate? with winnign thread
			agg2[omp_get_thread_num()] = il;
		}
	}
	//how to extract array besdt_ord

	int check_dist = 0;
	int correct_path = 0;
	for (i = 0; i < thread; i++)
	{
		for (j = 0; j < cities - 1; j++)
		{
			check_dist += arr[arr1[agg2[i]][j]][arr1[agg2[i]][j + 1]];
		}
		if (check_dist == best_distance)
		{
			correct_path = agg2[i];
			break;
		}
		check_dist = 0;
	}

	printf("Distance: %d\nBest path: ", best_distance);
	for (i = 0; i < cities - 1; i++)
	{
		printf("%lld ", arr1[correct_path][i]);
	}
	printf("%lld\n", arr1[correct_path][cities - 1]);

	free(arr1);		  //free 2d giant malloc of lexigraphical
	free(best_order); //when done
	free(arr);		  //free malloc
	fclose(fp);		  //close file down
	return 0;
}
