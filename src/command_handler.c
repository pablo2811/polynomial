#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

void PolyGetHelper(Stack *s, Poly *componentA, Poly *componentB, bool *err) {
    *componentA = popStack(s, err);
    *componentB = popStack(s, err);
}


bool zero(Stack *s) {
    Poly newPoly = PolyZero();
    pushStack(s, &newPoly);
}


void isCoeff(Stack *s, bool *err) {
    Poly top = topStack(s, err);
    if (*err) return;
    printf("%d\n", PolyIsCoeff(&top));
}

void isZero(Stack *s, bool *err) {
    Poly top = topStack(s, err);
    if (*err) return;
    printf("%d\n", PolyIsZero(&top));
}

void clone(Stack *s, bool *err) {
    Poly top = topStack(s, err);
    if (*err) return;
    Poly topClone = PolyClone(&top);
    pushStack(s, &topClone);
}

void add(Stack *s, bool *err) {
    Poly componentA, componentB, sum;
    PolyGetHelper(s, &componentA, &componentB, err);
    if (*err) return;
    sum = PolyAdd(&componentA, &componentB);
    PolyDestroy(&componentA);
    PolyDestroy(&componentB);
    pushStack(s, &sum);
}

void mul(Stack *s, bool *err) {
    Poly componentA, componentB, mulResult;
    PolyGetHelper(s, &componentA, &componentB, err);
    if (*err) return;
    mulResult = PolyMul(&componentA, &componentB);
    PolyDestroy(&componentA);
    PolyDestroy(&componentB);
    pushStack(s, &mulResult);
}

void neg(Stack *s, bool *err) {
    Poly toNeg = popStack(s, err);
    if (*err) return;
    Poly afterNeg = PolyNeg(&toNeg);
    PolyDestroy(&toNeg);
    pushStack(s, &afterNeg);
}

void sub(Stack *s, bool *err) {
    Poly componentA, componentB, subResult;
    PolyGetHelper(s, &componentA, &componentB, err);
    if (*err) return;
    subResult = PolySub(&componentA, &componentB);
    PolyDestroy(&componentA);
    PolyDestroy(&componentB);
    pushStack(s, &subResult);
}

void isEq(Stack *s, bool *err) {
    Poly componentA, componentB;
    PolyGetHelper(s, &componentA, &componentB, err);
    if (*err) return;
    printf("%d\n", PolyIsEq(&componentA, &componentB));
    pushStack(s, &componentB);
    pushStack(s, &componentA);
}

void deg(Stack *s, bool *err) {
    Poly top = topStack(s, err);
    if (*err) return;
    printf("%d\n", PolyDeg(&top));
}

void print(Stack *s, bool *err) {
    // what?
}

void pop(Stack *s, bool *err) {
    popStack(s, err);
}

void degBy(Stack *s, unsigned long long x, bool *err) {
    Poly top = topStack(s, err);
    if (*err) return;
    printf("%d\n", PolyDegBy(&top, x));
}

void at(Stack *s, long long x, bool *err) {
    Poly pop = popStack(s, err);
    if (*err) return;
    Poly atPoly = PolyAt(&pop, x);
    PolyDestroy(&pop);
    pushStack(s, &atPoly);
}



