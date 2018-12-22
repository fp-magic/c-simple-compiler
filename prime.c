#include<stdio.h>

int isPrime(int x)
{
    int i;
    int res;
    i = 2;
    if(x < 2)
    {
        res = 0;
    }
    else
    {
        res = 1;
    }
    while(i * i <= x)
    {
        if(x / i * i == x)
        {
            res = 0;
        }
        i = i + 1;
    }
    return res;
}

int main(void)
{
    int num;
    int res;
    scanf("%d", &num);
    if(isPrime(num) == 1)
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    printf("%d\n", res);
    return 0;
}
