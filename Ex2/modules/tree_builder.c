#include "tree_builder.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE_LENGTH 20

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

Node *createNode(){
    Node *node = (Node *)calloc(1, sizeof(Node)); // Initialized at 0 needs casting

    if (node == NULL)
    { //! If we allocate more than the system has f.example it returns null pointer
        fprintf(stderr, "Error : Failed memory allocation");
        exit(1);
    }

    // These two lines are redundant because calloc already initializes at 0 / NUll
    node->parent = NULL;
    node->one = NULL;
    node->zero = NULL; // We're in a leaf by default.
    node->frequency = 0; //*(float*)calloc(1,sizeof(float)); //Ig this ensures the float is on heap 
    return node;
}

int getHistogramLength(FILE* hist_file){

    char garbage[50]; // cant be char* garbage, else will dereference unvalid pointer, as that declaration doesn't assign a valid adress
    int histogram_length = 0;


    while(fgets(garbage,50,hist_file) != NULL){
        histogram_length++;
    }

    rewind(hist_file); // Needs to be reset as the file is passed by reference, not by value
    return histogram_length;
}

freq_histogram_line* histogramBuilder(FILE* hist_file){

    int hist_lengh = getHistogramLength(hist_file);
    freq_histogram_line* freq_histogram = (freq_histogram_line*)calloc(hist_lengh,sizeof(freq_histogram_line));

    int i = 0;
    long int num_of_chars = 0;

    char temp_s1[4]; //! numbers up to 255 need 4 bits, 3 for 3 char and '\0'
    char temp_s2[10];

    while(fscanf(hist_file,"%s %s ",temp_s1, temp_s2) != EOF){
        
        freq_histogram[i].symbol = atoi(temp_s1);
        freq_histogram[i].frequency = (float)atoi(temp_s2);

        num_of_chars += freq_histogram[i].frequency;
        i++;
    } 
    printf("Total chrs is %ld\n",num_of_chars);

    for(int i = 0;i<hist_lengh;i++){
        freq_histogram[i].frequency /= num_of_chars;
        printf("Character : %d\t\t Relative Frequency : %.3f\n",freq_histogram[i].symbol,freq_histogram[i].frequency);
    }

    return freq_histogram;
}

int down_frequency(const void* a, const void* b){
    // This function should order from smallest to highest frequency.
    // A small detail is that non yet assigned nodes should be put in the end of the array

    float float_tolerance = 0.00001;

    Node* elem_a = *(Node**)a;
    Node* elem_b = *(Node**)b;
    
    //* For debugging
    //printf("Node a adress is %p\n",elem_a);
    //printf("Node b adress is %p\n\n",elem_b);
    //printf("Node a frequency is %.3f\tNode b frequency is %.3f\n",elem_a->frequency,elem_b->frequency);
    
    // if(elem_a->frequency > elem_a->frequency){
    //    printf("Freq %f is bigger than %f\n\n",elem_a->frequency,elem_b->frequency);
    // }
    // else{
    //    printf("Freq %f is bigger than %f\n\n",elem_b->frequency,elem_a->frequency);
    // };

    if (fabs(elem_a->frequency - elem_b->frequency) < float_tolerance) return 0;

    return elem_a->frequency > elem_b->frequency ? 1 : -1; // 1 means a is bigger, -1 means b is bigger
}

char** inverseTreeBuilder(freq_histogram_line* frequency_histogram,int hist_length){

    float frequency_sum = 0; // Will keep track when the frequency_sum reaches 1

    // Array of pointers to node
    int dynamic_length = 2*hist_length-1;
    Node** node_pool = (Node**)calloc(dynamic_length,sizeof(Node*));
    Node** node_pointers = (Node**)calloc(dynamic_length,sizeof(Node*));

    char** code_array = (char**)malloc(hist_length*sizeof(char*)); // Well declaring them before using the above seems to get arround the overflow

    for (int i = 0; i < hist_length; i++) {
        code_array[i] = malloc(MAX_CODE_LENGTH * sizeof(char));
    }   


    if (node_pointers == NULL || node_pool == NULL){ //! If we allocate more than the system has f.example it returns null pointer
        fprintf(stderr, "Error : Failed memory allocation");
        exit(1);
    }
    //! An binary tree given n leaf nodes will have 2n-1 total nodes including root

    for(int i = 0;i<dynamic_length;i++){ // Creates every node
        node_pool[i] = createNode();
        node_pointers[i] = node_pool[i];  
    }

    for(int i = 0;i<hist_length;i++){
        node_pool[i]->symbol = frequency_histogram[i].symbol;
        node_pool[i]->frequency = frequency_histogram[i].frequency;
        // printf("Adress of node is %p\n", node_pool[i]);
        // printf("Symbol of node is %d\n", node_pool[i]->symbol);
        // printf("Frequency of node is %f\n", node_pool[i]->frequency);
    }

    printf("Dynamic length is %d\n",dynamic_length);
    printf("Number of leafs is %d\n",hist_length);
    
    int internal_node_idx = hist_length; // Position to insert internal nodes , might have to pass this outside loop 
    for(int i = 0; i<dynamic_length-1;i++){

        printf("\nAdress of node is %p\n", node_pool[i]);        
        printf("Symbol of node is %c\n", node_pool[i]->symbol);
        printf("Frequency of node->zero is %.3f\n", node_pool[i]->frequency);        
        printf("Adress of node->parent is %p\n", node_pool[i]->parent);        
        

        if(i%2 == 0){
            node_pointers[internal_node_idx]->one =node_pointers[i+1];
            node_pointers[internal_node_idx]->zero =node_pointers[i];
            node_pointers[internal_node_idx]->frequency =node_pointers[i]->frequency + node_pointers[i+1]->frequency;

            node_pointers[i]->parent = node_pointers[internal_node_idx];
            node_pointers[i+1]->parent =node_pointers[internal_node_idx];

            // There isn't ever a case where i+1 because the loop always has a odd number 
            // of iterations since theres always 2n-1 nodes, so the last time i&&2 will equal 0
            // is in the last-1 iteration, where will always be a i+1
            printf("\nAdress of inner node created is %p\n", node_pointers[internal_node_idx]);        
            printf("\nAdress of internal node->one is %p\n", node_pointers[internal_node_idx]->one);        
            printf("Adress of internal node->zero is %p\n", node_pointers[internal_node_idx]->zero);        
            printf("Frequency of internal node->zero is %.3f\n", node_pointers[internal_node_idx]->frequency);        
            
            //! Quicksort the array of pointers, not the array where the nodes are actually stored            
            qsort(node_pointers,dynamic_length,sizeof(Node*),down_frequency);

            if(internal_node_idx == dynamic_length){
                printf("Breaking\n");
                break;
            }

            internal_node_idx++;
        }
    }

    printf("\n\n\n\n\nFrequency orderered\n");
    for(int j = 0;j<dynamic_length;j++){
        printf("Frequency of node is %.3f\n",node_pointers[j]->frequency);
    }


    Node* root = node_pointers[dynamic_length-1];

    printf("Root adress is %p\n",root);
    printf("Root frequency is %f\n",root->frequency);
    printf("Root->one is %p\n",root->one);
    printf("Root->zero is %p\n",root->zero);




    //* By here the tree is already built, now will iterate the leaves up to root to find their codes

    Node * current_node;
    Node * past_node;

    int num_of_codes = hist_length;

    for(int i=0;i<hist_length;i++){
        //Hist length's index is the last leaf
        current_node = node_pool[i];
        past_node = node_pool[i];

        char code[MAX_CODE_LENGTH] = {};
        int code_building_idx = 0;

        printf("\n\nCode of symbol %c is: (inverted)\t",node_pool[i]->symbol);

        while(current_node->parent != NULL) { // If null means we are at root

            current_node = current_node->parent;
            if(current_node->one == past_node){
                // printf("1");
                code[code_building_idx] = '1';
            }
            else if(current_node->zero == past_node){
                // printf("0");
                code[code_building_idx] = '0';
            }
            code_building_idx++;
            past_node = current_node; // After comparing, i can update the past node
        }
        code[code_building_idx] = '\0'; // Null, to be interpreted as string

        strrev(code);
        strcpy(code_array[i],code); // Without this im assigning the heap adress to a stack adress
                                    // Also for this the code_array length must be defined, hence the for in lines 130
        printf("%s",code_array[i]);
    }


    free(node_pointers);
    free(node_pool);
    
    return code_array;
}

void codingTableWriter(FILE* coding_table,freq_histogram_line* histogram,int hist_length,char** code_array){


    printf("\n");
    for(int i = 0;i<hist_length;i++){
        // printf("%c\n",histogram[i].symbol);
        // printf("%s\n",code_array[i]);

        fputc(histogram[i].symbol,coding_table);
        fputc(32,coding_table); //Space
        fprintf(coding_table,"%s",code_array[i]);
        fputc(10,coding_table); // New line
    }

}

