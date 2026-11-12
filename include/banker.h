#ifndef BANKER_H
#define BANKER_H

extern int NUMBER_OF_CUSTOMERS; 
extern int NUMBER_OF_RESOURCES;  

extern int *available; 
extern int **maximum;     
extern int **allocation;  
extern int **need;        

void banker_init_resources(int number_of_resources);
void banker_free(void);

#endif 
