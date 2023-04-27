#include "foo.h"
#include <iostream>

Foo& globalFoo(){
    static Foo globalFoo(123, "globalFoo");
    return globalFoo;
}

int main(){
    printf("======main======\n");
}