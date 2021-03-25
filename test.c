#include "stdio.h"

int main()
{
    int *p, x;//local on stack --> can be any value
    
    p = &x; //lets p point to 'x' --> in memory
    *p = 5; //set pointer p = 5;

    printf("register: %x OR %x\np: %d\n", &x, p, x);

    return 0;
}