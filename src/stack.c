/** @file
  Implementacja operacji wykonywanych na stosie wielomianów rzadkich wielu zmiennych.

  @author Paweł Fijałkowski <pf429189@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include <stdlib.h>
#include "stack.h"

/**
 * Metoda sprawdzająca czy alokacja pamięci przeszła poprawnie.
 * @param ptr - wskaźnik na zaalokowaną pamięć.
 */
static void check_ptr(void *ptr) {
    if (ptr == NULL) {
        exit(1);
    }
}


Poly PopStack(Stack *s, bool *err) {
    if (s->amount == 0) {
        *err = true;
        return PolyZero();
    }

    (s->amount)--;
    return s->stack[s->amount];

}

Poly TopStack(Stack *s, bool *err) {
    if (s->amount == 0) {
        *err = true;
        return PolyZero();
    }

    return s->stack[s->amount - 1];
}

void PushStack(Stack *s, const Poly *p) {
    if (s->amount == s->capacity) {
        s->stack = realloc(s->stack, sizeof(Poly) * (2 * s->capacity));
        s->capacity *= 2;
        check_ptr(s->stack);
    }

    s->stack[s->amount] = *p;
    (s->amount)++;
}

Stack GetEmptyStack() {
    Stack s;

    s.stack = malloc(sizeof(Poly));
    check_ptr(s.stack);
    s.amount = 0;
    s.capacity = 1;

    return s;
}

