#include<stdio.h>

int a[50];
int main(void)
{
    int i;
    int n;
    scanf("%d", &n);
    a[0] = 0;
    a[1] = 1;

    i = 2;
    while(i <= n)
    {
        a[i] = a[i-1] + a[i-2];
        i = i + 1;
    }
    i = 1;
    while(i <= n)
    {
        printf("%d\n", a[i]);
        i = i + 1;
    }
    return 0;
}
