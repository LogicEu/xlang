#include <xc.h>
#include <string.h>

xc_stack_t xc_stack(unsigned int size, unsigned int bytes)
{
    xc_stack_t stack;
    stack.size = size;
    stack.bytes = bytes;
    stack.used = 0;
    stack.data = malloc(size * bytes);
    return stack;
}

xc_stack_t* xc_stack_new(unsigned int size, unsigned int bytes)
{
    xc_stack_t* stack = (xc_stack_t*)malloc(sizeof(xc_stack_t));
    stack->size = size;
    stack->bytes = bytes;
    stack->used = 0;
    stack->data = malloc(size * bytes);
    return stack;
}

unsigned int xc_stack_is_empty(const xc_stack_t* restrict stack)
{
    return stack->used == 0;
}

unsigned int xc_stack_is_full(const xc_stack_t* restrict stack)
{
    return stack->used == stack->size;
}

void* xc_stack_index(const xc_stack_t* restrict stack, unsigned int index)
{
    return (void*)((char*)stack->data + index * stack->bytes);
}

void xc_stack_resize(xc_stack_t* stack, unsigned int new_size)
{
    if (new_size < stack->used) return;
    stack->size = new_size;
    stack->data = realloc(stack->data, stack->size * stack->bytes);
}

void xc_stack_cut(xc_stack_t* stack)
{
    stack->size = stack->used;
    stack->data = realloc(stack->data, stack->size * stack->bytes);
}

void xc_stack_push(xc_stack_t* stack, void* data)
{
    if (stack->data == NULL) stack->data = malloc(stack->size * stack->used);
    if (stack->used == stack->size) xc_stack_resize(stack, stack->size * 2);
    memcpy(xc_stack_index(stack, stack->used++), data, stack->bytes);
}

void* xc_stack_pop(xc_stack_t* stack)
{
    if (stack->used == 0) return NULL;
    return xc_stack_index(stack, --stack->used);
}

void* xc_stack_peek(const xc_stack_t* restrict stack)
{
    if (stack->used == 0) return NULL;
    return xc_stack_index(stack, stack->used);
}

void xc_stack_free(xc_stack_t* stack)
{
    if (!stack || !stack->data) return;
    free(stack->data);
    stack->data = NULL;
    stack->used = 0;
}

void xc_stack_destroy(xc_stack_t* stack)
{
    if (stack == NULL) return;
    xc_stack_free(stack);
    free(stack);
}
