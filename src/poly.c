#include "poly.h"
#include <stdlib.h>

void insertMonoToPoly(Poly *p, Mono m) {
    p->arr = realloc(p->arr, (p->size + 1) * sizeof(Mono));
    p->arr[p->size] = m;
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

poly_exp_t comparator_exponents(const void *p1, const void *p2) {
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
    Poly p_copy;
    Mono m;
    m.p = PolyFromCoeff(x);
    m.exp = 0;
    p_copy.arr = malloc(sizeof(Mono));
    p_copy.size = 1;
    p_copy.arr[0] = m;
    return PolyAdd(p, &p_copy);
}

void Simplify(Poly *poly) {
    if (PolyIsCoeff(poly)) {
        return;
    }
    if (poly->size == 1 && poly->arr[0].exp == 0 && PolyIsCoeff(&poly->arr[0].p)) {
        poly_coeff_t coeff = poly->arr[0].p.coeff;
        MonoDestroy(poly->arr);
        poly->arr = NULL;
        poly->coeff = coeff;
        return;
    }
    Mono *simplified = NULL;
    size_t simplified_size = 0;
    for (size_t i = 0; i < poly->size; i++) {
        if (!PolyIsZero(&poly->arr[i].p)) {
            simplified = realloc(simplified, (simplified_size + 1) * sizeof(Mono));
            simplified[simplified_size] = MonoClone(poly->arr + i);
            simplified_size++;
        }
    }
    MonoDestroy(poly->arr);
    poly->arr = simplified;
    poly->size = simplified_size;
}


Poly PolyAdd(const Poly *p, const Poly *q) {
    Poly result = PolyFromCoeff(0);
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result.arr = NULL;
        result.coeff = p->coeff + q->coeff;
        return result;
    } else if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        result = addNumberToPoly(q, p->coeff);
    } else if (!PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result = addNumberToPoly(p, q->coeff);
    } else {
        size_t i, j;
        i = j = 0;
        while (i < p->size && j < q->size) {
            poly_exp_t exp_i = MonoGetExp(p->arr + i);
            poly_exp_t exp_j = MonoGetExp(q->arr + j);
            if (exp_i < exp_j) {
                insertMonoToPoly(&result, MonoClone(p->arr + i));
                i++;
            } else if (exp_j < exp_i) {
                insertMonoToPoly(&result, MonoClone(q->arr + j));
                j++;
            } else {
                Poly add_result = PolyAdd(&((p->arr + i)->p), &((q->arr + j)->p));
                if (!PolyIsZero(&add_result)) {
                    insertMonoToPoly(&result, MonoFromPoly(&add_result, MonoGetExp(p->arr + i)));
                }
                i++;
                j++;
            }
        }
        while (i < p->size) {
            insertMonoToPoly(&result, MonoClone(p->arr + i));
            i++;
        }
        while (j < q->size) {
            insertMonoToPoly(&result, MonoClone(q->arr + j));
            j++;
        }

    }
    Simplify(&result);
    return result;
}


Poly PolyAddMonos(size_t count, const Mono *monos) {
    Poly result;
    Mono *monos_copy = malloc(count * sizeof(Mono));
    for (size_t i = 0; i < count; i++) {
        monos_copy[i] = MonoClone(&monos[i]);
    }
    qsort(monos_copy, count, sizeof(Mono), comparator_exponents);
    size_t i = 0;
    Mono *temp = NULL;
    size_t size_temp = 0;
    while (i < count) {
        Poly current_sum = monos_copy[i].p;
        size_t j = i + 1;
        while (j < count && MonoGetExp(monos_copy + j) == MonoGetExp(monos_copy + i)) {
            Poly *ptr = &current_sum;
            Poly temp_poly = PolyAdd(&current_sum, &monos_copy[j].p);
            PolyDestroy(ptr);
            current_sum = temp_poly;
            j++;
        }
//        Simplify(&current_sum);
        if (!PolyIsZero(&current_sum)) {
            temp = realloc(temp, (size_temp + 1) * sizeof(Mono));
            temp[size_temp] = MonoFromPoly(&current_sum, monos_copy[i].exp);
            size_temp++;
        }
        i = j;
    }
//    for(size_t t = 0; t < count; t++){
//        MonoDestroy(&monos_copy[t]);
//    }

    result.arr = temp;
    result.size = size_temp;
    Simplify(&result);
    return result;
}

Poly PolyCoefMul(poly_coeff_t m, const Poly *q) { // q - nie liczba, p - liczba
    Poly result;
    result.arr = malloc((q->size) * sizeof(Mono));
    for (size_t i = 0; i < q->size; i++) {
        Poly number = PolyFromCoeff(m);
        result.arr[i].p = PolyMul(&(q->arr->p), &number);
        result.arr[i].exp = q->arr->exp;
    }
    result.size = q->size;
    Simplify(&result);
    return result;
}

Poly PolyMul(const Poly *p, const Poly *q) {
    Poly result;
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result.arr = NULL;
        result.coeff = p->coeff * q->coeff;
    } else if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        return PolyCoefMul(p->coeff, q);
    } else if (!PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyCoefMul(q->coeff, p);
    } else {
        Mono *tmp = malloc((p->size) * (q->size) * sizeof(Mono));
        for (size_t i = 0; i < p->size; i++) {
            for (size_t j = 0; j < q->size; j++) {
                Poly multiplication_result = PolyMul(&((p->arr + i)->p), &((q->arr + j)->p));
                poly_exp_t exponent = MonoGetExp(p->arr + i) + MonoGetExp(q->arr + j);
                if (!PolyIsZero(&multiplication_result)) {
                    tmp[i * q->size + j] = MonoFromPoly(&multiplication_result, exponent);
                }
            }
        }
        result = PolyAddMonos((p->size) * (q->size), tmp);
        MonoDestroy(tmp);
        Simplify(&result);
        return result;
    }
    Simplify(&result);
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

poly_exp_t MonoDeg(Mono *m) {
    if (PolyIsCoeff(&(m->p))) {
        return MonoGetExp(m);
    }
    return PolyDeg(&(m->p)) + MonoGetExp(m);
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsZero(p)) {
        return -1;
    }
    if (PolyIsCoeff(p)) {
        return 0;
    }
    poly_exp_t deg = -1;
    for (size_t i = 0; i < p->size; i++) {
        poly_exp_t current;
        if ((current = MonoDeg(p->arr + i)) > deg) {
            deg = current;
        }
    }
    return deg;
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


poly_coeff_t notQuickPower(poly_coeff_t x, poly_exp_t exp) {
    poly_coeff_t result = 1;
    for (poly_exp_t i = 0; i < exp; i++) {
        result *= x;
    }
    return result;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    Poly result = PolyFromCoeff(0);;
    if (PolyIsCoeff(p)) {
        result = PolyFromCoeff(p->coeff);
        return result;
    }
    Poly current;
    for (size_t i = 0; i < p->size; i++) {
        poly_exp_t my_exponent = p->arr[i].exp;
        poly_coeff_t constant = notQuickPower(x, my_exponent);
        Poly multiplier = PolyFromCoeff(constant);
        current = PolyMul(&multiplier, &p->arr[i].p);
//        Poly *ptr = &result;
        result = PolyAdd(&result, &current);
//        PolyDestroy(ptr);
    }
    return result;
}




