#include <stdio.h>
#include <stdlib.h>

int main() {
    
    void *ptr = malloc(1024);
    
    if (ptr == NULL) {
       
        printf("Memory allocation failed\n");
        return 1;
    }
  
    printf("Memory allocated successfully\n");
    
    free(ptr);
    
    return 0;
}
