#include <stdio.h>
#include<string.h>
#include <stdlib.h>
int main(void)
{
    char address[8];
    unsigned long array[64]={0};
    int hit=0;
    int number=0;
    while(scanf("%s",address) && strcmp(address,"END")!=0)
    {
        ++number;
        char tag_str[6];
        strncpy(tag_str,address,5);
        unsigned long tag=strtoul(tag_str,NULL, 16);
        tag=strtoul(tag_str,NULL, 16)&0xFFFFC;
        char index_str[3];
        strncpy(index_str,address+4,2);
        unsigned long index=strtoul(index_str,NULL,16)&0x3F;
        if(array[index]==tag)
            {
                hit++;
                puts("Hit");
            }
        else
        {
            puts("Miss");
            array[index]=tag;
        }
    }
    printf("Hit ratio = %.2f%%", (double)hit/number*100);
    return 0;
}
