#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define NUM 300
#define sample 2

int duty[NUM];
int peak[NUM / 2];
int count[NUM / 2];
int toF[NUM / 2];

void dutyCycle();
void peakSort();
void tof();
void printval();

int main()
{
    /*
        - array[] record amplitude 1 or 0
        - duty cycle will be stored in another array from 0-6
            - count number of 1's within 6 samples and stor
              in new array with an integer value from 0-6
        - detect peak amplitudes from stored array of integers
            - check values until >= to determine peak
            - track the index number peak is detected at
            - each index number is 1/6MHz
                - store index number and peak and determine difference
                  in peak to find ToF
    */
    FILE *fptr;

    srand(time(0)); //random seed generator

    dutyCycle(); //generate duty cycle in array with rand()
    peakSort();  //find the peak values
    tof();       //find time of flight
    printval();

    fptr = fopen("export.txt", "w");

    if (!fptr)
    {
        printf("Unable to open file for write\n");
        return;
    }
    while (!feof(fptr))
    {
        fgets(peak, sizeof(peak), fptr);
        fprintf(fptr, "%d", peak);
    }

    return 0;
}

void dutyCycle()
{
    for (int i = 0; i < NUM; i++)
    {
        int upper = 6;
        int lower = 0;
        int num = (rand() % (upper - lower + 1) + lower);
        duty[i] = num;
    }
}

void peakSort()
{
    int j = 0;
    for (int i = 1; i < NUM; i++)
    {
        if ((duty[i] > duty[i - 1]) && (duty[i] > duty[i + 1]))
        {
            int num;
            count[j] = i;      //store index number to save
            peak[j] = duty[i]; //store peak value from duty[] into peak[]
            j++;               //increment to next element
        }
        else
        {
            printf("%d ", i);
            ;
        }
    }
}

void tof()
{
    /*
        - have peak value
        - have index counter for each peak
            - every index is differed by sample rate
    */
    for (int i = 0; i < sizeof(count) / sizeof(count[0]); i++)
    {
        int temp;
        temp = count[i + 1] - count[i];
        toF[i] = abs(temp * sample);
    }
}

void printval()
{
    printf("\nDuty Cycle: ");
    for (int i = 0; i < NUM; i++)
    {
        printf("%d ", duty[i]); //print duty cycles
    }

    printf("\n\npeak: "); //
    for (int i = 0; i < sizeof(peak) / sizeof(peak[0]); i++)
    {
        printf("%d ", peak[i]); //print peak values
    }

    printf("\n\nindex: ");
    for (int i = 0; i < sizeof(count) / sizeof(count[0]); i++)
    {
        printf("%d ", count[i]); //print index value
    }

    printf("\n\nToF: "); // -----
    for (int i = 0; i < sizeof(count) / sizeof(count[0]); i++)
    {
        printf("%d ", toF[i]); //print index value
    }
    printf("\n"); // -----
}