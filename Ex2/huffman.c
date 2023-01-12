#include "modules/tree_builder.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){

    FILE* hist_file = fopen(argv[1],"r");
    FILE* coding_table = fopen("coding_table.txt","w");
    int histogram_length = getHistogramLength(hist_file);
    
    freq_histogram_line* histogram = histogramBuilder(hist_file);

    char** code_array = inverseTreeBuilder(histogram,histogram_length);

    codingTableWriter(coding_table,histogram,histogram_length,code_array);


}





