#include <stdio.h>
#include <stdlib.h>



char* DupString(const char* str){

    int len = 0;

    while(str[len] != '\0'){
        len++;
    }

    char* dup_string = (char*)malloc(sizeof(char)*len+1);

    if(dup_string == NULL){ // If exceds maximum pc memmory
        fprintf(stderr,"Error allocating memory");
    }

    for(int i = 0;i<len;i++){
        dup_string[i] = str[i];
    }
    
    dup_string[len+1] = '\0';

    return dup_string;
}

int main(int argc,char* argv[]){

    char* string = argv[1];

    char* copy = DupString(string);

    printf("Copied string is %s\n",copy);

    printf("Original string adress is %p\nCopied string adress is%p",&string,&copy);

    return 0;
}