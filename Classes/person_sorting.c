#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct p{
    char* name;
    unsigned age;
}person_struct;

int up_age(const void* a, const void* b){
// pointer typecasting is necessary, otherwise comparing dereferenced void pointers is wrong because the compiler
// doesn't know how long each variable is
    return ((person_struct*)a)->age > ((person_struct*)b)->age ? 1 : -1; // 1 means a is bigger, -1 means b is bigger
}

int down_age(const void* a, const void* b){
    return ((person_struct*)a)->age < ((person_struct*)b)->age ? 1 : -1; // 1 means a is bigger, -1 means b is bigger
}

int up_name(const void* a, const void* b){
    //First compares by ascii code, then by size
    // Size wise, shorter is greater
    // Returns 1 if string a is bigger than b, -1 otherwise
    return strcmp(((person_struct*)a)->name,((person_struct*)b)->name); // 1 means a is bigger, -1 means b is bigger
}


//! Here it matters if they're equal

int main(){

    // person_struct persons[5] = {}; // Initializes everything at 0
    // person_struct persons[5]; // Initializes garbage
    person_struct persons[] = {{"Maria",12},{"Manuel",23},{"Ana",12}}; // Initializers

    qsort(persons,3,sizeof(person_struct),up_name);

    for(int i = 0; i<3; i++){
        printf("%s , %d\n",persons[i].name,persons[i].age);
    }
    
}