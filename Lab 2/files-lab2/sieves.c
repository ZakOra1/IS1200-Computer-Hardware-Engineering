#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

void print_number(int n)
{
  static int counter;
  printf("%10d ", n);
  counter++;
  if(counter % COLUMNS == 0)
  {
    printf("\n"); 
  }
}

void print_sieves(int n)
{
    int integers[n-1];
    int counter;
    int totalSpaces = 0;

    //Fill the array with integers 2 to n
    for(int i = 0; i < n-1; i++)
    {
        integers[i] = i+2;
    }

    int dividingPrime;

    //Mark multiples of prime numbers
    for(int i = 0; i <= n-1; i++)
    {
        if(integers[i] != 0)
        {
            dividingPrime = integers[i];
        }

        for(int j = 2 ; j < n-1; j++)
        {
            if(integers[j] % dividingPrime == 0 && integers[j] != dividingPrime)
            {
                integers[j] = 0;
            }
        }

    }
    for(int i = 0; i < n-1; i++)
    {
        if(integers[i] == 0)
        {
            counter++;
        }
        else{
            if(counter == 3 )
            {
                totalSpaces++;
            }
            counter = 0;
        }
    }
    //Write out prime numbers
    for(int i = 0; i <= n-2; i++)
    {
        if(integers[i] != 0)
        {
            print_number(integers[i]);
        }
    }
    printf("\n"); 
    printf("Number of primes with exactly a distance of 4 between them: ");
    printf("%2d ", totalSpaces);
}

int main(int argc, char *argv[]){
    if(argc == 2)
        print_sieves(atoi(argv[1]));
    else
        printf("Please state an interger number.\n");
    return 0;
}