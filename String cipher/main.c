/*
 * Stream Cipher
 * Author: Moming
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define num 70000

//  g(x) = 1 + x + x^2 + x^15 + x^16;

int BinToDec(int vec[]);
void DecToBin(int n, int vec[]);
int exist(int vec[], int n);
void Display(int vec[]);

int main()
{
    int OriginKey []={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    int Seq[70000] = {0}, Stream[16], flag = 0, temp = 0, mod = 0, i, Key[70000] = {0};
    temp = BinToDec(OriginKey);
    Seq[flag] = temp;
    //printf("temp = %d\n", temp);
    for(i = 0; i < 16; i++)
    {
        Stream[i] = OriginKey[i];
        //printf("@%d\t", Stream[i]);
    }
    while(flag < num)
    {
        mod = (Stream[0] + Stream[1] + Stream[2] + Stream[15]) % 2;
        Key[flag++] = mod;
        //printf("mod = %d\n", mod);
        for(i = 0; i < 15; i++)
        {
            Stream[i] = Stream[i + 1];
        }
        Stream[15] = mod;
        temp = BinToDec(Stream);
        //printf("temp = %d\n", temp);
        if(!exist(Seq, temp))
        {
            Seq[flag] = temp;
        }
        else
        {
            printf("End!\n");
            break;
        }
    }
    printf("Cycle: %d\n", flag);

    printf("Top 100: \n");
    for(i = 0; i < 100; i++)
    {
        //DecToBin(Seq[i], Stream);
        //Display(Stream);
        printf("%d ", Key[i]);
    }
    printf("\n");
    return 0;
}

int exist(int vec[], int n)
{
    int i;
    for(i = 0; i < num; i++)
    {
        if(n == vec[i])
        {
            printf("Duplicates: %d\n", n);
            return 1;
        }
        if(vec[i] == 0)
        {
            break;
        }
    }
    return 0;
}

int BinToDec(int vec[])
{
    int i, n = 0;
    for(i = 0; i < 16; i++)
    {
        n += vec[i] * (int)(pow(2, 15 - i));
    }
    return n;
}

void DecToBin(int n, int vec[])
{
    int i;
    for(i = 0; i < 16; i++)
    {
        if(n >= (int)pow(2, 15 - i))
        {
            n -= (int)pow(2, 15 - i);
            vec[i] = 1;
        }
        else
        {
            vec[i] = 0;
        }
    }
}

void Display(int vec[])
{
    int i;
    for(i = 0; i < 16; i++)
    {
        printf("%d ", vec[i]);
    }
    printf("\n");
}
