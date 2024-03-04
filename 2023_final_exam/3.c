#include <stdio.h>
#define N 100
void Delete(char *str);

void Delete(char *str)
{
    char *slow = str, *fast = str + 1;
    int count;
    while (*fast != '\0')
    {
        char *p = str;
        count = 0;
        while (p <= slow)
        {
            if (*p == *fast)
                count++;
            if (count == 2)
                break;
            else
                p++;
        }
        if (p == slow + 1)
        {
            *p = *fast;
            slow++;
        }
        fast++;
    }
    *(++slow) = '\0';
}

int main()
{
    char str[N] = "abaccbadcdaacbdab";
    Delete(str);
    printf("%s\n", str);
    return 0;
}