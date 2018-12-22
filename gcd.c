#include<stdio.h>

int gcd(int x, int y)
{
    if(y == 0)
    {
        return x;
    }
    else
    {
        return gcd(y, x-x/y*y);
    }
    return 0;
}

int main(void)
{
    int a;
    int b;
    int c;
    scanf("%d", &a);
    scanf("%d", &b);
    c = gcd(a, b);
    printf("%d\n", c);
    return 0;
}
