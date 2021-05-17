#include "poly.h"

#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H

#endif //POLYNOMIALS_STACK_H

typedef struct Stack {
    Poly *stack;
    int capacity;
    int amount;
} Stack;

Poly PopStack(Stack *s, bool *err);

Poly TopStack(Stack *s, bool *err);

void PushStack(Stack *s, const Poly *p);

Stack GetEmptyStack();
