#include "modules/tree_builder.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){

    FILE* hist_file = fopen(argv[1],"r");
    int histogram_length = getHistogramLength(hist_file);
    
    freq_histogram_line* histogram = histogramBuilder(hist_file);

    Node* root = inverseTreeBuilder(histogram,histogram_length);

}