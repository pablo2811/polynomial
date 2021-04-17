#include "poly.h"
#include <stdlib.h>
#include <stdio.h>


void insertMonoToPoly(Poly *p, Mono m) {
    p->arr = realloc(p->arr, (p->size + 1) * sizeof(Mono));
    p->arr[p->size] = m;
    size_t i = p->size;
    while (i >= 1 && MonoGetExp(&(p->arr[i])) < MonoGetExp(&(p->arr[i - 1]))) {
        Mono tmp = p->arr[i - 1];
        p->arr[i - 1] = p->arr[i];
        p->arr[i] = tmp;
        i--;
    }
    (p->size)++;
}

size_t max(size_t a, size_t b) {
    return (a > b) ? a : b;
}

size_t min(size_t a, size_t b) {
    return (a > b) ? b : a;
}


void PolyDestroy(Poly *p) {
    if (p != NULL && p->arr != NULL) {
        for (size_t i = 0; i < p->size; ++i) {
            MonoDestroy(&p->arr[i]);
        }
        free(p->arr);
    }
}

int comparator_exponents(const void *p1, const void *p2) {
    Mono *mono1 = (Mono *) p1;
    Mono *mono2 = (Mono *) p2;
    return mono1->exp - mono2->exp;
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

Poly addNumberToPoly(const Poly *p, poly_coeff_t x) {
    Poly result = PolyClone(p);
    result.arr = realloc(result.arr, (result.size + 1) * sizeof(Mono));
    Poly q = PolyFromCoeff(x);
    Mono m = MonoFromPoly(&q, 0);
    result.arr[result.size] = m;
    (result.size)++;
    qsort(result.arr, result.size, sizeof(Mono), comparator_exponents);
    return result;
}

void Simplify(Poly *poly) {
    if (PolyIsCoeff(poly)) {
        return;
    }
    Mono *simplified = NULL;
    size_t simplified_size = 0;
    bool poly_zero = true;
    for (size_t i = 0; i < poly->size; i++) {
        if (!PolyIsZero(&poly->arr[i].p)) {
            simplified = realloc(simplified, (simplified_size + 1) * sizeof(Mono));
            simplified[simplified_size] = MonoClone(poly->arr + i);
            simplified_size++;
//            poly_zero = false;
        }
    }
//    if (poly_zero && !PolyIsCoeff(poly)) {
//        poly->coeff = 0;
//    }
    MonoDestroy(poly->arr);
    poly->arr = simplified;
    poly->size = simplified_size;
}


Poly PolyAdd(const Poly *p, const Poly *q) {
    Poly result;
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result.arr = NULL;
        result.coeff = p->coeff + q->coeff;
        return result;
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
    Poly *old_result = &result;
    Poly final_result = PolyAddMonos(result.size, result.arr);
    PolyDestroy(old_result);
    Simplify(&final_result);
    return final_result;
}


Poly PolyAddMonos(size_t count, const Mono *monos) {
    Poly result;
    result.arr = malloc(count * sizeof(Mono));
    for (size_t i = 0; i < count; i++) {
        result.arr[i] = MonoClone(&monos[i]);
    }
    qsort(result.arr, count, sizeof(Mono), comparator_exponents);
    size_t i = 0;
    Mono *temp = NULL;
    size_t size_temp = 0;
    while (i < count) {
        Poly current_sum = result.arr[i].p;
        size_t j = i + 1;
        while (j < count && MonoGetExp(result.arr + j) == MonoGetExp(result.arr + i)) {
            Poly *ptr = &current_sum;
            Poly temp_poly = PolyAdd(&current_sum, &result.arr[j].p);
            PolyDestroy(ptr);
            current_sum = temp_poly;
            j++;
        }
        Simplify(&current_sum);
        if (!PolyIsZero(&current_sum)) {
            temp = realloc(temp, (size_temp + 1) * sizeof(Mono));
            temp[size_temp] = MonoFromPoly(&current_sum, result.arr[i].exp);
            size_temp++;
        }
        i = j;
    }

    if (size_temp == 1 && temp->exp == 0 && PolyIsCoeff(&temp[0].p)) {
        result.arr = NULL;
        result.coeff = temp[0].p.coeff;
        return result;
    }
//    MonoDestroy(result.arr);
//    if (size_temp == 1 && temp->exp == 0) {
//        result = PolyFromCoeff(temp->p.coeff);
//    } else {
    result.arr = temp;
    result.size = size_temp;
//    }
//    qsort(result.arr, count, sizeof(Mono), comparator_exponents);
    return result;
}

Poly PolyCoefMul(const Poly *p, const Poly *q) {
    Poly result;
    result.arr = malloc((q->size) * sizeof(Mono));
    for (size_t i = 0; i < q->size; i++) {
        Poly number = PolyFromCoeff(p->coeff);
        result.arr[i].p = PolyMul(&(q->arr->p), &number);
        result.arr[i].exp = q->arr->exp;
    }
    result.size = q->size;
    return result;
}

Poly PolyMul(const Poly *p, const Poly *q) {
    Poly result;
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result.arr = NULL;
        result.coeff = p->coeff * q->coeff;
    } else if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        return PolyCoefMul(p, q);
    } else if (!PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyCoefMul(q, p);
    } else {
        result.arr = malloc((p->size) * (q->size) * sizeof(Mono));
        result.size = (p->size) * (q->size);
        for (size_t i = 0; i < p->size; i++) {
            for (size_t j = 0; j < q->size; j++) {
                result.arr[i * q->size + j].p = PolyMul(&((p->arr + i)->p), &((q->arr + j)->p));
                result.arr[i * q->size + j].exp = MonoGetExp(p->arr + i) + MonoGetExp(q->arr + j);
            }
        }
        Poly *old_result = &result;
        Poly final_result = PolyAddMonos(result.size, result.arr);
        PolyDestroy(old_result);
        Simplify(&final_result);
        return final_result;
    }

    return result;
}


Poly PolyNeg(const Poly *p) {
    Poly result;
    if (PolyIsCoeff(p)) {
        result.coeff = -p->coeff;
        result.arr = NULL;
    } else {
        result.arr = malloc(sizeof(Mono) * (p->size));
        result.size = p->size;
        for (size_t i = 0; i < p->size; i++) {
            result.arr[i].p = PolyNeg(&p->arr[i].p);
            result.arr[i].exp = (p->arr + i)->exp;
        }
    }
    return result;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly negative = PolyNeg(q);
    Poly sum = PolyAdd(p, &negative);
    PolyDestroy(&negative);
    return sum;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsCoeff(p)) {
        return (PolyIsZero(p)) ? -1 : 0;
    }
    if (var_idx == 0) {
        poly_exp_t max_down = 0;
        for (size_t i = 0; i < p->size; i++) {
            poly_exp_t current;
            if ((current = MonoGetExp(p->arr + i)) > max_down) {
                max_down = current;
            }
        }
        return max_down;
    } else {
        poly_exp_t max = 0;
        for (size_t i = 0; i < p->size; i++) {
            poly_exp_t current;
            if ((current = PolyDegBy(&p->arr[i].p, var_idx - 1)) > max) {
                max = current;
            }
        }
        return max;
    }
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
        size_t max = p->size;
        size_t i = 0;
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




