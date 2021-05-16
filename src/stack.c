#include <stdlib.h>
#include "stack.h"

#define CHECK_PTR(p) \
    do {         \
        if (p == NULL) { \
            exit(1); \
        } \
    } while (0) \


Poly popStack(Stack *s, bool *err) {
    if (s->amount == 0) {
        *err = true;
        return PolyZero();
    }
    (s->amount)--;
    return s->stack[s->amount];

}

Poly topStack(Stack *s, bool *err) {
    if (s->amount == 0) {
        *err = true;
        return PolyZero();
    }
    return s->stack[s->amount - 1];
}

void pushStack(Stack *s, const Poly *p) {
    if (s->amount == s->capacity) {
        s->stack = realloc(s->stack, sizeof(Poly) * (2 * s->capacity));
        s->capacity *= 2;
        CHECK_PTR(s->stack);
    }
    s->stack[s->amount] = *p;
    (s->amount)++;
}

Stack getEmptyStack() {
    Stack s;
    s.stack = malloc(sizeof(Poly));
    CHECK_PTR(s.stack);
    s.amount = 0;
    s.capacity = 1;
    return s;
}

