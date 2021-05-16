#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

void PolyGetHelper(Stack *s, Poly *componentA, Poly *componentB, bool *err) {
    *componentA = PopStack(s, err);
    if (*err) return;
    *componentB = PopStack(s, err);
    if (*err) PushStack(s, componentA);
}


void zero(Stack *s) {
    Poly newPoly = PolyZero();
    PushStack(s, &newPoly);
}


void isCoeff(Stack *s, bool *err) {
    Poly top = TopStack(s, err);
    if (*err) return;
    printf("%d\n", PolyIsCoeff(&top));
}

void isZero(Stack *s, bool *err) {
    Poly top = TopStack(s, err);
    if (*err) return;
    printf("%d\n", PolyIsZero(&top));
}

void clone(Stack *s, bool *err) {
    Poly top = TopStack(s, err);
    if (*err) return;
    Poly topClone = PolyClone(&top);
    PushStack(s, &topClone);
}

void add(Stack *s, bool *err) {
    Poly componentA, componentB, sum;
    PolyGetHelper(s, &componentA, &componentB, err);
    if (*err) return;
    sum = PolyAdd(&componentA, &componentB);
    PolyDestroy(&componentA);
    PolyDestroy(&componentB);
    PushStack(s, &sum);
}

void mul(Stack *s, bool *err) {
    Poly componentA, componentB, mulResult;
    PolyGetHelper(s, &componentA, &componentB, err);
    if (*err) return;
    mulResult = PolyMul(&componentA, &componentB);
    PolyDestroy(&componentA);
    PolyDestroy(&componentB);
    PushStack(s, &mulResult);
}

void neg(Stack *s, bool *err) {
    Poly toNeg = PopStack(s, err);
    if (*err) return;
    Poly afterNeg = PolyNeg(&toNeg);
    PolyDestroy(&toNeg);
    PushStack(s, &afterNeg);
}

void sub(Stack *s, bool *err) {
    Poly componentA, componentB, subResult;
    PolyGetHelper(s, &componentA, &componentB, err);
    if (*err) return;
    subResult = PolySub(&componentA, &componentB);
    PolyDestroy(&componentA);
    PolyDestroy(&componentB);
    PushStack(s, &subResult);
}

void isEq(Stack *s, bool *err) {
    Poly componentA, componentB;
    PolyGetHelper(s, &componentA, &componentB, err);
    if (*err) return;
    printf("%d\n", PolyIsEq(&componentA, &componentB));
    PushStack(s, &componentB);
    PushStack(s, &componentA);
}

void deg(Stack *s, bool *err) {
    Poly top = TopStack(s, err);
    if (*err) return;
    printf("%d\n", PolyDeg(&top));
}

void print(Stack *s, bool *err) {
    Poly top = TopStack(s, err);
    if (*err) return;
    PolyPrint(&top);
}

void pop(Stack *s, bool *err) {
    PopStack(s, err);
}

void degBy(Stack *s, unsigned long long x, bool *err) {
    Poly top = TopStack(s, err);
    if (*err) return;
    printf("%d\n", PolyDegBy(&top, x));
}

void at(Stack *s, long long x, bool *err) {
    Poly pop = PopStack(s, err);
    if (*err) return;
    Poly atPoly = PolyAt(&pop, x);
    PolyDestroy(&pop);
    PushStack(s, &atPoly);
}



