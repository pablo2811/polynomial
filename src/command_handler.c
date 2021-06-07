/** @file
  Skypt zawierający funkcje wykonujące polecenia użytkownika kalkulatora wielomianów

  @author Paweł Fijałkowski <pf429189@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "stack.h"

/**
 * Jeśli to możliwe, pobiera ze stosu, dwa wielomiany i przypisuje do `componentA`, `componentB`.
 * Ustawia err = true, jeśli na stosie nie ma conajmniej dwóch elementów.
 * @param[in] s : Stack
 * @param[in] componentA : Poly
 * @param[in] componentB : Poly
 * @param[in] err : bool
 */

static void PolyGetHelper(Stack *s, Poly *componentA, Poly *componentB, bool *err) {
    *componentA = PopStack(s, err);
    if (*err) {
        return;
    }

    *componentB = PopStack(s, err);
    if (*err) {
        PushStack(s, componentA);
    }
}


void Zero(Stack *s) {
    Poly newPoly = PolyZero();
    PushStack(s, &newPoly);
}


void IsCoefficient(Stack *s, bool *err) {
    Poly top = TopStack(s, err);

    if (*err) {
        return;
    }

    printf("%d\n", PolyIsCoeff(&top));
}

void IsZero(Stack *s, bool *err) {
    Poly top = TopStack(s, err);

    if (*err) {
        return;
    }

    printf("%d\n", PolyIsZero(&top));
}

void Clone(Stack *s, bool *err) {
    Poly top = TopStack(s, err);

    if (*err) {
        return;
    }

    Poly topClone = PolyClone(&top);
    PushStack(s, &topClone);
}

void Add(Stack *s, bool *err) {
    Poly componentA, componentB, sum;
    PolyGetHelper(s, &componentA, &componentB, err);

    if (*err) {
        return;
    }

    sum = PolyAdd(&componentA, &componentB);
    PolyDestroy(&componentA);
    PolyDestroy(&componentB);

    PushStack(s, &sum);
}

void Mul(Stack *s, bool *err) {
    Poly componentA, componentB, mulResult;
    PolyGetHelper(s, &componentA, &componentB, err);

    if (*err) {
        return;
    }

    mulResult = PolyMul(&componentA, &componentB);
    PolyDestroy(&componentA);
    PolyDestroy(&componentB);

    PushStack(s, &mulResult);
}

void Neg(Stack *s, bool *err) {
    Poly toNeg = PopStack(s, err);

    if (*err) {
        return;
    }

    Poly afterNeg = PolyNeg(&toNeg);
    PolyDestroy(&toNeg);

    PushStack(s, &afterNeg);
}

void Sub(Stack *s, bool *err) {
    Poly componentA, componentB, subResult;
    PolyGetHelper(s, &componentA, &componentB, err);

    if (*err) {
        return;
    }

    subResult = PolySub(&componentA, &componentB);
    PolyDestroy(&componentA);
    PolyDestroy(&componentB);

    PushStack(s, &subResult);
}

void IsEq(Stack *s, bool *err) {
    Poly componentA, componentB;
    PolyGetHelper(s, &componentA, &componentB, err);

    if (*err) {
        return;
    }

    printf("%d\n", PolyIsEq(&componentA, &componentB));

    PushStack(s, &componentB);
    PushStack(s, &componentA);
}

void Deg(Stack *s, bool *err) {
    Poly top = TopStack(s, err);

    if (*err) {
        return;
    }

    printf("%d\n", PolyDeg(&top));
}

void Print(Stack *s, bool *err) {
    Poly top = TopStack(s, err);

    if (*err) {
        return;
    }

    PolyPrint(&top);
}

void Pop(Stack *s, bool *err) {
    PopStack(s, err);
}

void DegBy(Stack *s, unsigned long long x, bool *err) {
    Poly top = TopStack(s, err);

    if (*err) {
        return;
    }

    printf("%d\n", PolyDegBy(&top, x));
}

void At(Stack *s, long long x, bool *err) {
    Poly pop = PopStack(s, err);

    if (*err) {
        return;
    }

    Poly atPoly = PolyAt(&pop, x);
    PolyDestroy(&pop);
    PushStack(s, &atPoly);
}

void Compose(Stack *s, unsigned long long k, bool *err) {

    if (s->amount - 1 < k) {
        *err = true;
        return;
    }

    Poly main = PopStack(s, err);
    Poly *q = malloc(k * sizeof(Poly));
    for (int u = (int) k - 1; u >= 0; u--) {
        q[u] = PopStack(s, err);
    }

    Poly result = PolyCompose(&main, k, q);
    PushStack(s, &result);

    for (size_t u = 0; u < k; u++) {
        PolyDestroy(q + u);
    }
    PolyDestroy(&main);
    free(q);

}



