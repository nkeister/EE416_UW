#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define NUM 300 //size of array
#define NUM2 2
#define FREQUENCY 6000000 //frequency

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
        - using NUM2 = 4 vs NUM2 = 2
            - removes 4-7% of the data
            - saves 30% of memory
            - NUM2 = 4 causes seg fault at 100,000 values
            - NUM4 = 2 allows up to storage space.
            SOLUTION
                - using some sort of vector to allocate memory in 
                  array when needed

*/
int duty[NUM];
int peak[NUM / NUM2];
int count[NUM / NUM2];
int toF[NUM / NUM2];
int x[NUM / NUM2];

void dutyCycle();
void peakSort();
void tof();
void printval();
void export();

int main()
{
    int temp2;

    srand(time(0)); //random seed generator

    dutyCycle(); //generate duty cycle in array with rand()
    peakSort();  //find the peak values
    tof();       //find time of flight
    printval();  //print data
    export();

    return 0;
}

void dutyCycle()
{
    for (int i = 0; i < NUM; i++)
    {
        x[i] = i;
        int upper = 6;                                    //upper duty cycle limit
        int lower = 0;                                    //lower duty cycel limit
        int num = (rand() % (upper - lower + 1) + lower); //gen rand #
        duty[i] = num;                                    //store num in duty[]
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
            //printf("%d ", i);
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
            - toF should be half of total toF
    */
    for (int i = 0; i < sizeof(count) / sizeof(count[0]); i++)
    {
        int temp;
        temp = count[i + 1] - count[i];
        toF[i] = abs(temp * FREQUENCY) / 2;
    }
}

void printval()
{
    printf("counter toF    index\n"); //-----
    for (int i = 0; i < sizeof(peak) / sizeof(peak[0]); i++)
    {
        printf("%d        %d     %d\n", x[i], count[i], toF[i]); //print peak values
    }
}

void export()
{
    FILE *outfile = fopen("export.txt", "w+");

    if (outfile == NULL)
    {
        printf("Unable to open file for write\n");
        exit(1);
    }

    for (int i = 0; i < sizeof(count) / sizeof(count[0]); i++)
    {
        fprintf(outfile, "%d %d %d\n", peak[i], toF[i], count[i]); //data to be exported
    }

    fclose(outfile); //close file
}