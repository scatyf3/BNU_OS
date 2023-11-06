#include "shared_memory.h"
#include <stdio.h>

int main(){
    char* string;
    string=get_shared_block(FILENAME,10*sizeof(char));
    return 0;
}