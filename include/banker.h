#ifndef BANKER_H
#define BANKER_H
extern int NUMBER_OF_CUSTOMERS;  
extern int NUMBER_OF_RESOURCES;  
extern int *available;    
extern int **maximum;     
extern int **allocation;  
extern int **need;      

enum {
    REQUEST_GRANTED = 0,      
    REQUEST_EXCEEDS_NEED,     
    REQUEST_NOT_AVAILABLE,    
    REQUEST_UNSAFE            
};

void banker_init_resources(int number_of_resources);
int *banker_add_customer(void);
void initialize_state(void);
int request_resources(int customer_num, const int request[]);
void banker_free(void);

#endif 
