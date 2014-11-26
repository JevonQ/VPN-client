#include <stdio.h>
#include <string.h>

#define C1 52845
#define C2 22719
#define C3 1213

void encrypt(const char *in, char *result)
{
    int size = strlen(in);
    char input[100] = {'\0'};
    char str[2] = {'1', '2'};
    int i,j, k=0;
    int key = C3;

    (void) memset(result, '\0', sizeof (result));
    strcpy(result, in);
    strcpy(input, in);
    printf("the input string length is %d\n", size);
    for(i = 0; i < size; i ++)
    {
       result[i] = input[i]^(key>>8);
       key = ((unsigned char)result[i]+key)*C1+C2;
    }
    strcpy(input, result);
    (void) memset(result, '\0', sizeof (result));

    for(i=0; i< (int)strlen(input); i++)
    {
       j = (unsigned char)input[i];
       str[0] = 65+j/26;
       str[1] = 65+j%26;
       result[k++] = str[0];
       result[k++] = str[1];
    }
    result[k] = '\0';
}


void decrypt(const char *in, char *result)
{
    int size = strlen(in);
    char input[100] = {'\0'};
    char str[1] = {'1'};
    int i,j, k = 0;
    int key = C3;

    (void) memset(result, '\0', sizeof (result));
    strcpy(input, in);
    for (i=0; i < size/2; i++)
    {
       j = ((unsigned char)input[2*i]-65)*26;
       j += (unsigned char)input[2*i+1]-65;
       str[0] = j;
       result[k++] = str[0];
    }
    result[k] = '\0';
    memset(input, '\0', sizeof (input));
    strcpy(input, result);
    for (i=0; i< (int) strlen(input); i++)
    {
       result[i] = (unsigned char)input[i]^(key>>8);
       key = ((unsigned char)input[i]+key)*C1+C2;
    }
}


int main()
{
    const char str[] = "abcdef";
    char enresult[100] = {'\0'};
    char deresult[100] = {'\0'};
    printf("the input string length is %d\n", sizeof(str));
    encrypt(str, enresult);
    printf("after encrypt, the result is %s\n", enresult);
    decrypt(enresult, deresult);
    printf("after encrypt, the result is %s\n", deresult);
    return 0;
}

