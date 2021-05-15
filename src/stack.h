#include "poly.h"

#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H

#endif //POLYNOMIALS_STACK_H

typedef struct Stack {
    Poly *stack;
    int capacity;
    int amount;
} Stack;

Poly popStack(Stack *s, bool *err);

Poly topStack(Stack *s, bool *err);

void pushStack(Stack *s, const Poly *p);

Stack getEmptyStack();

