#ifndef VAT_MODULE_H
#define VAT_MODULE_H

// Function pointer type
typedef int (*module_function_t)(int, char**);

// Module struct type
typedef struct {
    char long_name[25];
    char short_name[3];
    char description[100];
    module_function_t function;
} module_t;

#endif