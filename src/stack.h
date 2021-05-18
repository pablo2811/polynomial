#include "poly.h"

#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H

#endif //POLYNOMIALS_STACK_H

/**
 * Struktura reprezentująca stos przechowywanych w programie wielomianów.
 * Zawiera informację o rozmiarze (capacity), ilości przechowywanych wielomianów (amount)
 * i samych wielomianach (stack).
 */
typedef struct Stack {
    Poly *stack;
    int capacity;
    int amount;
} Stack;

/**
 * Metoda usuwająca i zwracająca element z wierzchołka stosu.
 * @param s - Stack, stos do wykonania operacji
 * @param err - informacja o błędzie
 * @return poly: wielomian z wierzchołka stosu
 */
Poly PopStack(Stack *s, bool *err);

/**
 * Metoda zwracająca element z wierzchołka stosu.
 * @param s - Stack, stos do wykonania operacji
 * @param err - informacja o błędzie
 * @return poly: wielomian z wierzchołka stosu
 */
Poly TopStack(Stack *s, bool *err);

/**
 * Metoda dodająca wielomian na wierzchołek stosu.
 * @param s - Stack, stos do wykonania operacji
 * @param p - wielomian do dodania.
 */
void PushStack(Stack *s, const Poly *p);

/**
 * Metoda zwracająca pusty stos.
 * @return Stack - pusty stos.
 */
Stack GetEmptyStack();
