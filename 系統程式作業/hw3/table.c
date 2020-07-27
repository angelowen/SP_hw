#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define size 10000//Array size
long table[size][size];
long col[size];
long row[size];

void initTable()
{
	int i, j;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			table[i][j] = random();//put random number to the table
}

void sum()
{
	int i, j;
	for (j = 0; j < size; j++){
		
		for(i=0;i<size;i++){
			col[j]+=table[i][j];//put the sum into col[j] and continue to add the next number
			row[i]+=table[i][j];	//put the sum into row[j] and continue to add the next number

		}
	
	}
}

void printResult()//print the result
{
	int i;
	printf("     RAW\tCol\n");
	for (i = 0; i < size; i++)
		printf("%8ld\t%8ld\n", row[i], col[i]);
}

int main()
{
	printf("hello\n");
	initTable();//put the random to the table array
	for (int i = 0; i < 1000000; i++)
	{
		sum();//add row and col into 1D array
	}
	printResult();//print the result
	return 0;
}
