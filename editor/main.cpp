#include "AttributeContainer.h"
#include "AttributeContainer.cpp"

int main(){

    AttributeContainer<double> numbers;

    double * number0 = numbers.AddElement(0);

    return 0;
}
