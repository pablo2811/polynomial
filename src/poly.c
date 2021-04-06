#include "poly.h"
#include <stdlib.h>
#include <stdio.h>

Poly addNumberToPoly(const Poly *p, poly_coeff_t x) {
    Poly result = PolyClone(p);
    result.arr = realloc(result.arr, (result.size + 1) * sizeof(Mono));
    Poly q = PolyFromCoeff(x);
    Mono m = MonoFromPoly(&q, 0);
    result.arr[result.size] = m;
    (result.size)++;
    return result;
}

void insertMonoToPoly(Poly *p, Mono m) {
    p->arr = realloc(p->arr, (p->size + 1) * sizeof(Mono));
    p->arr[p->size] = m;
    int i = p->size;
    while (i >= 1 && MonoGetExp(&(p->arr[i])) < MonoGetExp(&(p->arr[i - 1]))) {
        Mono tmp = p->arr[i - 1];
        p->arr[i - 1] = p->arr[i];
        p->arr[i] = tmp;
        i--;
    }
    (p->size)++;
}

size_t max(poly_exp_t a, poly_exp_t b) {
    return (a > b) ? a : b;
}

size_t min(poly_exp_t a, poly_exp_t b) {
    return (a > b) ? b : a;
}


void PolyDestroy(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; i++) {
            PolyDestroy(&((p->arr[i]).p));
        }
        MonoDestroy(p->arr);
    }
}

Poly PolyClone(const Poly *p) {
    Poly result;
    if (PolyIsCoeff(p)) {
        result.coeff = p->coeff;
        result.arr = NULL;
    } else {
        result.arr = malloc(sizeof(Mono) * (p->size));
        for (size_t i = 0; i < p->size; i++) {
            result.arr[i] = MonoClone(p->arr + i);
        }
        result.size = p->size;
    }
    return result;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    Poly result;
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result.arr = NULL;
        result.coeff = p->coeff + q->coeff;
    } else if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        result = addNumberToPoly(q, p->coeff);
    } else if (!PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result = addNumberToPoly(p, q->coeff);
    } else {
        result.arr = NULL;
        result.size = 0;
        for (size_t i = 0; i < min(p->size, q->size); i++) {
            if (MonoGetExp(p->arr + i) == MonoGetExp(q->arr + i)) {
                result.arr = realloc(result.arr, (result.size + 1) * sizeof(Mono));
                result.arr[result.size].p = PolyAdd(&((p->arr + i)->p), &((q->arr + i)->p));
                result.arr[result.size].exp = MonoGetExp(p->arr + i);
                (result.size)++;
            } else {
                insertMonoToPoly(&result, MonoClone(p->arr + i));
                insertMonoToPoly(&result, MonoClone(q->arr + i));
            }
        }
        for (size_t j = min(p->size, q->size); j < max(p->size, q->size); j++) {
            if (p->size > q->size) {
                insertMonoToPoly(&result, MonoClone(p->arr + j));
            } else {
                insertMonoToPoly(&result, MonoClone(q->arr + j));
            }
        }
    }
    return result;
}

Poly PolyAddMonos(size_t count, const Mono *monos) {
    Poly result;
    return result;
}

Poly PolyMul(const Poly *p, const Poly *q) {
    Poly result;
    return result;
}

Poly PolyNeg(const Poly *p) {
    Poly result;
    if(PolyIsCoeff(p)){
        result.coeff *= (-1);
        result.arr = NULL;
    }else{
        result.arr = malloc(sizeof(Mono)*(p->size));
        for(size_t i = 0; i < p->size; i++){
            result.arr[i].p = PolyNeg(&((p->arr+i)->p));
            result.arr[i].exp = (p->arr+i)->exp;
        }
    }
    return result;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly result;
    return result;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    return 0;
}

poly_exp_t PolyDeg(const Poly *p) {
    return 0;
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return (p->coeff == q->coeff);
    }
    if (p->arr != NULL && q->arr != NULL) {
        if (p->size != q->size) {
            return false;
        }
        int max = p->size;
        int i = 0;
        bool equal = true;
        while (i < max && equal) {
            equal = PolyIsEq(&((p->arr[i]).p), &((q->arr[i]).p)) && MonoGetExp(p->arr + i) == MonoGetExp(q->arr + i);
            i++;
        }
        return (i == max && equal);
    }
    return false;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    Poly result;
    return result;
}




