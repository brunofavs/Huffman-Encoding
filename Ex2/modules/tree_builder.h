#ifndef TREE_BUILDER_H
#define TREE_BUILDER_H
#include <stdio.h>


typedef struct node
{
    struct node *parent;
    struct node *one; //! This isn't recursive
    struct node *zero;
    float frequency;
    //! Causes malloc() corrupted top size for some reason
    char symbol;
} Node;


typedef struct h_l{
    int symbol;
    float frequency; // TODO Keep in mind this 2**10 max of each char
}freq_histogram_line;

Node *createNode();

int getHistogramLength(FILE* hist_file);

freq_histogram_line* histogramBuilder(FILE* hist_file);

Node* inverseTreeBuilder(freq_histogram_line* frequency_histogram,int hist_length);


#endif