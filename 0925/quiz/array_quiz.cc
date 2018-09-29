/* array1.c */
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int i, a[10];
    int sum = 0;
    for (i = 0; i < 10; i++)
    {
        a[i] = i;
        sum += i;
    }
    printf("sum = %d\n", sum);
    exit(0);
}
