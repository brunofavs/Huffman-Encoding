#include <math.h>
#include "modules/tree_builder.h"
#include <stdio.h>
#include <stdlib.h>

int down_freq(const void* a, const void* b){

    float float_tolerance = 0.00001;
        
    Node* elem_a = *(Node**)a;
    Node* elem_b = *(Node**)b;
    
    // Node* elem_a = (Node*)a;
    // Node* elem_b = (Node*)b;

    printf("Node a adress is %p\n",elem_a);
    printf("Node b adress is %p\n\n",elem_b);
    // printf("Node a frequency is %.5f\t\t\tNode b frequency is %.5f\n",elem_a->frequency,elem_b->frequency);
    
    // if(elem_a->frequency > elem_a->frequency){
    //     printf("Freq %f is bigger than %f\n\n",elem_a->frequency,elem_a->frequency);
    // }
    // else{
    //     printf("Freq %f is bigger than %f\n\n",elem_a->frequency,elem_a->frequency);
    // };


    // If either value is 0, treat it as greater than the other value
    if (fabs(elem_a->frequency) - 0.0 < float_tolerance) return 1;// a = 0, means A is bigger, so 1
    if (fabs(elem_b->frequency) - 0.0 < float_tolerance) return -1;// b = 0, means B is bigger so -1
    if (fabs(elem_a->frequency - elem_b->frequency) < float_tolerance) return 0;



    return elem_a->frequency > elem_b->frequency ? 1 : -1; // 1 means a is bigger, -1 means b is bigger
}

int main(int argc,char* argv[]){

    int dynamic_length = 5;
    Node** node_pool = (Node**)calloc(dynamic_length,sizeof(Node*));
    Node** node_pointers = (Node**)calloc(dynamic_length,sizeof(Node*));


    for(int i = 0;i<dynamic_length;i++){ // Creates every node
        node_pool[i] = createNode();
        printf("Adresses outside function %p\n", node_pool[i]);
    }

    printf("\tPrinting adresses before qsort\n");
    for(int j = 0;j<dynamic_length;j++){ // Creating a array with the adresses of the nodes
        node_pointers[j] = node_pool[j];  
        printf("Adresses outside function %p\n", node_pointers[j]);
    }

        qsort(node_pointers,dynamic_length,sizeof(Node*),down_freq);


}

