/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>

int is_prime(int n){
  int counter = 0;
  int res = 0;
  for(int i = 1; i <= n; i++) 
  {
    if(n % i == 0)
    {
      counter++;
    }
  }
  if (counter == 2)
  {
    res = 1;
  }

  return res;
}

int main(void){
  printf("%d\n", is_prime(0));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(1)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(2)); // 987 is not a prime. Should print 0.
}
