#include "AttributeContainer.h"

#include <stdio.h>

int main(){

    AttributeContainer<double> numbers;

    double * number0 = numbers.AddElement(0);

    if( number0 ){
        printf("Element added successfully!\n");
    }else{  
        printf("Element addition failed\n");
    }

    return 0;
}
