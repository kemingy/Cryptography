/*
 * Data Encryption Standard
 * 2015-11-27
 * @Author Moming
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define fPath "Plaintext.txt"

const int num = 16;
const int length = 64;
const int cdlength = 28;
const int keylength = 48;
const int lrlength = 32;

void HexToBinary(int Hex[], int Binary[]);
void BinaryToHex(int Binary[], int Hex[]);
int IntPow(int x, int n);
void GetSubkey(int key[], int Subkey[][keylength]);
void LeftMove(int x[], int y[], int n);
void Replace(int C[], int D[], int key[]);
void Combine(int C[], int D[], int key[]);
void Encrypt(int Plaintext[], int Subkey[][keylength], int Ciphertext[]);
void ReplaceIP(int Plaintext[], int L[], int R[]);
void ArrayCopy(int x[], int y[]);
void EncryptCore(int output[], int L[], int R[], int K[]);
void InverseReplaceIP(int output[], int L[], int R[]);
void ESelect(int output[], int x[]);
void Sbox(int x[], int y[]);
void Reorder(int x[], int output[]);
void Decrypt(int Ciphertext[], int Subkey[][keylength], int DePlaintext[]);
void Display(int x[], int n);
void DecToBinary(int x[], int n);

int main()
{
    int i, Key[num], result[num], j, k, len, pos;
    int OriginKey[length], OriginPlaintext[length], Ciphertext[length];
    int Subkey[num][keylength];

    char input[1000];
    FILE *fp = NULL;

    if((fp = fopen(fPath, "r")) == NULL)
    {
        printf("Can't open file!\n");
        exit(1);
    }

    for(i = 0; (input[i] = fgetc(fp)) != EOF; i++){}

    fclose(fp);

    len = i;
    int cycle = ceil(len / 8.0);
    int plain[cycle * 64], temp[len], cmd[8];
    pos = 0;

    for(i = 0; i < len; i++)
    {
        temp[i] = (int)(input[i]);
        DecToBinary(cmd, temp[i]);
        for(j = 0; j < 8; j++, pos++)
        {
            plain[pos] = cmd[j];
        }
    }
    for(; pos < cycle * 64; pos++)
    {
        plain[pos] = 0;
    }

    printf("Enter your 16 bits hexadecimal key:\n");

    for(i = 0; i < num; i++)
    {
        scanf("%x", &Key[i]);
    }

    HexToBinary(Key, OriginKey);
    GetSubkey(OriginKey, Subkey);

    printf("\nThe Ciphertext is:\n");

    for(i = 0; i < cycle; i++)
    {
        for(j = 0; j < 64; j++)
        {
            OriginPlaintext[j] = plain[i * 64 + j];
        }

        Encrypt(OriginPlaintext, Subkey, Ciphertext);

        BinaryToHex(Ciphertext, result);
        for(k = 0; k < 16; k++)
        {
            printf("%x", result[k]);
        }
    }

    printf("\n");
    return 0;
}

/* Dec to Binary */
void DecToBinary(int x[], int n)
{
    int i, num = n;
    x[0] = 0;
    for(i = 1; i < 8; i++)
    {
        if(num >= IntPow(2, 8 - i - 1))
        {
            x[i] = 1;
            num -= IntPow(2, 8 - i - 1);
        }
        else
        {
            x[i] = 0;
        }
    }
}

/* x^n */
int IntPow(int x, int n)
{
    int result = 1;

    while(n-- > 0)
    {
        result *= x;
    }

    return result;
}

/* transform hex to binary */
void HexToBinary(int Hex[], int Binary[])
{
    int i = 0, j = 0;

    for(i = 0; i < num; i++)
    {
        for(j = 0; j < 4; j++)
        {
            if(Hex[i] >= IntPow(2, (3 - j)))
            {
                Binary[i * 4 + j] = 1;
                Hex[i] -= IntPow(2, (3 - j));
            }
            else
            {
                Binary[i * 4 + j] = 0;
            }
        }
    }
}

/* transform binary to hex */
void BinaryToHex(int Binary[], int Hex[])
{
    int i = 0;

    for(i = 0; i < num; i++)
    {
        Hex[i] = Binary[i * 4] * 8 + Binary[i * 4 + 1] * 4 + Binary[i * 4 + 2] * 2 + Binary[i * 4 + 3];
    }
}

/* get the subkey */
void GetSubkey(int key[], int Subkey[][keylength])
{
    int C[num + 1][cdlength], D[num + 1][cdlength];
    int i = 0;
    int Left[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    Replace(C[0], D[0], key);

    for(i = 0; i < num; i++)
    {
        LeftMove(C[i], C[i + 1], Left[i]);
        LeftMove(D[i], D[i + 1], Left[i]);
        Combine(C[i + 1], D[i + 1], Subkey[i]);
    }
}

/* y = x << n */
void LeftMove(int x[], int y[], int n)
{
    int i = 0;

    for(i = 0; i < cdlength; i++)
    {
        y[i] = x[(i + n) % 28];
    }
}

/* Replacement choose 1 */
void Replace(int C[], int D[], int key[])
{
    int C0[] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
                10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36
               };
    int D0[] = {63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
                14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
               };
    int i = 0;

    for(i = 0; i < cdlength; i++)
    {
        C[i] = key[C0[i] - 1];
        D[i] = key[D0[i] - 1];
    }
}

/* combine C and D to Key */
void Combine(int C[], int D[], int key[])
{
    int i = 0, temp[cdlength * 2];
    int R[] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12,
               4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55,
               30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42,
               50, 36, 29, 32
              };

    for(i = 0; i < cdlength; i++)
    {
        temp[i] = C[i];
    }

    for(i = 0; i < cdlength; i++)
    {
        temp[i + cdlength] = D[i];
    }

    for(i = 0; i < keylength; i++)
    {
        key[i] = temp[R[i] - 1];
    }
}

/* Encrypt plaintext with subkey */
void Encrypt(int Plaintext[], int Subkey[][keylength], int Ciphertext[])
{
    int L[num + 1][lrlength], R[num + 1][lrlength];
    int i = 0;
    ReplaceIP(Plaintext, L[0], R[0]);

    for(i = 0; i < num; i++)
    {
        ArrayCopy(L[i + 1], R[i]);
        EncryptCore(R[i + 1], L[i], R[i], Subkey[i]);
    }

    InverseReplaceIP(Ciphertext, L[num], R[num]);
}

/* Origin replace ip */
void ReplaceIP(int Plaintext[], int L[], int R[])
{
    int temp[64], i = 0;
    int ReplaceIp[] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
                       62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
                       57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
                       61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
                      };

    for(i = 0; i < length; i++)
    {
        temp[i] = Plaintext[ReplaceIp[i] - 1];
    }

    for(i = 0; i < lrlength; i++)
    {
        L[i] =  temp[i];
    }

    for(i = 0; i < lrlength; i++)
    {
        R[i] =  temp[i + lrlength];
    }
}

/* copy y's elements to x */
void ArrayCopy(int x[], int y[])
{
    int i = 0;

    for(i = 0; i < lrlength; i++)
    {
        x[i] = y[i];
    }
}

/* the core of DES */
void EncryptCore(int output[], int L[], int R[], int K[])
{
    int select[keylength], temp[keylength], temporary[keylength], i = 0;
    ESelect(select, R);

    for(i = 0; i < keylength; i++)
    {
        temp[i] = select[i] ^ K[i];
    }

    Sbox(temp, temporary);
    Reorder(temporary, output);

    for(i = 0; i < keylength; i++)
    {
        output[i] = output[i] ^ L[i];
    }
}

/* Inverse permutation */
void InverseReplaceIP(int output[], int L[], int R[])
{
    int InverseIP[] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
                       38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
                       36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
                       34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
                      };
    int temp[length], i = 0;

    for(i = 0; i < lrlength; i++)
    {
        temp[i] = R[i];
    }

    for(i = 0; i < lrlength; i++)
    {
        temp[i + lrlength] = L[i];
    }

    for(i = 0; i < length; i++)
    {
        output[i] = temp[InverseIP[i] - 1];
    }
}

/* E select operation */
void ESelect(int output[], int x[])
{
    int i = 0;
    int E[] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
               8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
               16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
               24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
              };

    for(i = 0; i < keylength; i++)
    {
        output[i] = x[E[i] - 1];
    }
}

/* S-Box */
void Sbox(int x[], int y[])
{
    int i = 0, j = 0, row = 0, col = 0, temp = 0;
    int S[8][64] = {{
            14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
            0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
            4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
            15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
        },
        {   15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
            3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
            0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
            13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
        },
        {   10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
            13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
            13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
            1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
        },
        {   7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
            13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
            10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
            3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
        },
        {   2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
            14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
            4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
            11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
        },
        {   12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
            10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
            9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
            4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
        },
        {   4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
            13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
            1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
            6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
        },
        {   13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
            1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
            7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
            2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
        }
    };

    for(i = 0; i < 8; i++)
    {
        row = x[6 * i] * 2 + x[6 * i + 5];
        col = x[6 * i + 1] * 8 + x[6 * i + 2] * 4 + x[6 * i + 3] * 2 + x[6 * i + 4];
        temp = S[i][16 * row + col];

        for(j = 0; j < 4; j++)
        {
            if(temp >= IntPow(2, (3 - j)))
            {
                y[4 * i + j] = 1;
                temp -= IntPow(2, (3 - j));
            }
            else
            {
                y[4 * i + j] = 0;
            }
        }
    }
}

/* reorder the matrix */
void Reorder(int x[], int output[])
{
    int i = 0;
    int P[] = {16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
               2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
              };

    for(i = 0; i < lrlength; i++)
    {
        output[i] = x[P[i] - 1];
    }
}

/* Dncrypt plaintext with subkey */
void Decrypt(int Ciphertext[], int Subkey[][keylength], int DePlaintext[])
{
    int L[num + 1][lrlength], R[num + 1][lrlength];
    int i = 0;
    ReplaceIP(Ciphertext, L[0], R[0]);

    for(i = 0; i < num; i++)
    {
        ArrayCopy(L[i + 1], R[i]);
        EncryptCore(R[i + 1], L[i], R[i], Subkey[num - i - 1]);
    }

    InverseReplaceIP(DePlaintext, L[num], R[num]);
}

/* test the output */
void Display(int x[], int n)
{
    int i = 0, logo = 0;

    for(i = 0; i < n; i++)
    {
        printf("%d", x[i]);
        logo++;

        if(logo >= 8)
        {
            logo = 0;
            printf(" ");
        }
    }

    printf("\n");
}
