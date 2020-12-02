#include <stdio.h>
#include <stdint.h>


int main(void){

    uint32_t var1;      // 부호확장 안됨
    int32_t var2;       // 부호확장 됨

    var1 = 0xffffffff;
    var2 = 0xffffffff;

    printf("uint32_t : %d\nint32_t : %d\n\n", var1, var2);    

    var1 >>= 8;
    var2 >>= 8;

    printf("uint32_t : %x\nint32_t : %x\n\n", var1, var2);    


    var1 |= 0xf0000000;

    printf("uint32_t : %x\nint32_t : %x\n\n", var1, var2);    

    return 0;
}