#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

//? How to print extended ASCII characters

int buffer_size = 30; // Global variable, not a macro since will need to be updated

typedef struct h_l{
    char symbol;
    int count;
}histogram_line;

int up_count(const void* a, const void* b);


int main(int argc,char* argv[]){

    if(argc != 2){
        printf("[ERROR] Please input only the name of the source file\n");
        exit(1);
    }

    FILE* source_file = fopen(argv[1],"r");
    
    // Initial buffer size
    histogram_line* histogram= (histogram_line*)calloc(buffer_size,sizeof(histogram_line));

    int histogram_empty_idx = 0;
    int symbol_in_array = FALSE;

    //* Looping once to find unique characters
    for(int read_chr = fgetc(source_file), i = 0; read_chr!= EOF;read_chr = fgetc(source_file),i++){
        
        //*Checking if read_chr exists in hist
       for(int n = 0;n<buffer_size;n++){
            if(histogram[n].symbol == read_chr){
                symbol_in_array = TRUE; // If there isn't the symbol anywhere, it should return false
                break;                  // If the character exists, theres no point in keep checking
            }
        }
        // * Assigning new element to histogram
        if(symbol_in_array == FALSE){

            //*Checking if needs to allocate more memmory
            if(histogram_empty_idx == buffer_size){
                printf("Buffer limit reached, allocating more memory\n");
                buffer_size*=2;
                histogram =(histogram_line*)realloc(histogram,sizeof(histogram_line)*buffer_size);
            }

            histogram[histogram_empty_idx].symbol = read_chr;
            histogram_empty_idx++;
            // printf("Adding character %c to histogram\n",read_chr);
        }
        symbol_in_array = FALSE; // Reseting variable for next iteration
    } 
    rewind(source_file);

    //* Looping again to determine count

    for(int read_chr = fgetc(source_file); read_chr!= EOF;read_chr = fgetc(source_file)){
       for(int n = 0;n<buffer_size;n++){
            if(histogram[n].symbol == read_chr){
                histogram[n].count++;
            }
        }
    }

    //* Sorting by count
    qsort(histogram,buffer_size,sizeof(histogram_line),up_count);

    //* Printing histogram
    for(int n = 0;n<buffer_size;n++){
        if(histogram[n].symbol == 0 || histogram[n].symbol == 10)
            continue;
        printf("Character: %c (%d)",histogram[n].symbol,histogram[n].symbol);
        printf("\tCount: %d\n",histogram[n].count);
    }

    //* Writting to outfile.

    FILE* out_file = fopen("out_file.txt","w");


    for(int n = 0;n<buffer_size;n++){
        if(histogram[n].symbol == 0 || histogram[n].symbol == 10)
            continue; // Because of empty buffer

        // fputc(histogram[n].symbol,out_file); // To print letters to outfile
        fprintf(out_file, "%d", histogram[n].symbol); 
        fputc(9,out_file); // 9 is ascii for \t
        fprintf(out_file, "%d", histogram[n].count); 
        fputc(10,out_file); // 10 is ascii for \n
    }

}


int up_count(const void* a, const void* b){
    return ((histogram_line*)a)->count > ((histogram_line*)b)->count ? 1 : -1;
}