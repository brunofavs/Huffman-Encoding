#include <stdio.h>
#include <stdlib.h>

char* DecToBin(unsigned int num){

    int number_of_bits = sizeof(unsigned)*8;

    char* binary_number = (char*)malloc(number_of_bits+1);
    
    for(int n = number_of_bits;n>=0;n--){
        binary_number[number_of_bits-n] = num & 1<<n ? '1' : '0';
        // printf("%c",binary_number[number_of_bits - n]);
    }

    binary_number[number_of_bits+1] = '\0';
     
    return binary_number;
}

int main(int argc,char* argv[]){

    int num = 10;

    char * bin = DecToBin(num);

    printf("Number in binary is %s\n",bin);

}