#ifndef VAT_MODULE_H
#define VAT_MODULE_H

// Function pointer type
typedef int (*module_function_t)(int, char**);

// Module struct type
typedef struct {
    char long_name[50];
    char short_name[5];
    module_function_t function;
} module_t;

#endif