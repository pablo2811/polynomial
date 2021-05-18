/** @file
  Implementacja klasy wielomianów rzadkich wielu zmiennych.

  @author Paweł Fijałkowski <pf429189@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "poly.h"
#include <stdlib.h>
#include <stdio.h>

void PolyPrintUtil(const Poly *poly);

/**
 * Metoda sprawdzająca czy alokacja pamięci przeszła poprawnie.
 * @param ptr - wskaźnik na zaalokowaną pamięć.
 */
static void check_ptr(void *ptr) {
    if (ptr == NULL) {
        exit(1);
    }
}

/**
 * Funkcja wypisująca podany jako parametr jednomian.
 * @param mono - Mono, jednomian do wypisania.
 */
void MonoPrint(const Mono *mono) {
    printf("(");
    PolyPrintUtil(&(mono->p));
    printf(",%d)", mono->exp);

}

/**
 * Funkcja pomocnicza wykonująca obliczenia na rzecz `PolyPrint`.
 * Wypisuje podany jako parametr - wielomian, bez znaku końca linii.
 * @param poly - wielomian do wypisania bez znaku końca linii.
 */
void PolyPrintUtil(const Poly *poly) {
    if (PolyIsCoeff(poly)) {
        printf("%ld", poly->coeff);
        return;
    }

    for (size_t i = 0; i < poly->size; i++) {
        MonoPrint(poly->arr + i);
        if (i < poly->size - 1) {
            printf("+");
        }
    }

}

void PolyPrint(const Poly *p) {
    PolyPrintUtil(p);
    printf("\n");

}

void InsertMonoToPoly(Poly *poly, Mono *m) {
    poly->arr = realloc(poly->arr, (poly->size + 1) * sizeof(Mono));
    check_ptr(poly->arr);

    (poly->arr)[poly->size] = *m;
    (poly->size)++;
}

void PolyDestroy(Poly *p) {
    if (p != NULL && p->arr != NULL) {

        for (size_t i = 0; i < p->size; ++i) {
            MonoDestroy(&p->arr[i]);
        }
        free(p->arr);
    }

}

/**
 * Dodaje nowy jednomian do wielomianu.
 * @param[in] p1 : wskaźnik rzutowalny na jednomianowy.
 * @param[in] p2 : wskaźnik rzutowalny na jednomianowy.
 * @return Różnica wartości wykładników jednomianów.
 */
static poly_exp_t ComparatorExponents(const void *p1, const void *p2) {
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
        if (p->size == 0) {
            return PolyFromCoeff(p->coeff);
        }

        result.arr = malloc(sizeof(Mono) * (p->size));
        check_ptr(result.arr);

        for (size_t i = 0; i < p->size; i++) {
            result.arr[i] = MonoClone(p->arr + i);
        }
        result.size = p->size;
    }

    return result;
}

/**
 * Dodaje liczbę do wielomianu.
 * @param[in] p : Wielomian do którego należy dodać liczbę.
 * @param[in] x : Liczba (współczynnik) do dodania.
 * @return Wielomian będący wynikiem dodawania.
 */
Poly AddNumberToPoly(const Poly *p, poly_coeff_t x) {
    Poly p_copy;
    Mono m;

    m.p = PolyFromCoeff(x);
    m.exp = 0;

    p_copy.arr = malloc(sizeof(Mono));
    check_ptr(p_copy.arr);
    p_copy.size = 1;
    p_copy.arr[0] = m;

    Poly result = PolyAdd(p, &p_copy);
    PolyDestroy(&p_copy);

    return result;
}

/**
 * Usuwa zerowe jednomiany z wielomianu.
 * @param[in] poly : Wielomian do uproszczenia.
 */
static void Simplify(Poly *poly) {
    if (PolyIsCoeff(poly)) return;

    for (size_t i = 0; i < poly->size; i++) {
        Simplify(&poly->arr[i].p);
    }

    size_t swapper = poly->size - 1;
    size_t i = 0;

    while (i <= swapper) {
        if (PolyIsZero(&poly->arr[i].p)) {
            poly->arr[i] = poly->arr[swapper];
            (poly->size)--;
            if (swapper == 0) {
                break;
            }
            swapper--;
        } else {
            i++;
        }
    }

    if (poly->size == 0) {
        PolyDestroy(poly);
        *poly = PolyZero();
    } else if (poly->size == 1 && poly->arr[0].exp == 0 && PolyIsCoeff(&poly->arr[0].p)) {
        Poly res = PolyFromCoeff(poly->arr[0].p.coeff);
        PolyDestroy(poly);
        *poly = res;
    } else {
        qsort(poly->arr, poly->size, sizeof(Mono), ComparatorExponents);
    }

}


Poly PolyAdd(const Poly *p, const Poly *q) {
    Poly result = PolyZero();

    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result.arr = NULL;
        result.coeff = p->coeff + q->coeff;
        return result;
    } else if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        result = AddNumberToPoly(q, p->coeff);
    } else if (!PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result = AddNumberToPoly(p, q->coeff);
    } else {
        size_t i, j;
        i = j = 0;

        while (i < p->size && j < q->size) {
            poly_exp_t exp_i = MonoGetExp(p->arr + i);
            poly_exp_t exp_j = MonoGetExp(q->arr + j);

            if (exp_i < exp_j) {
                Mono my_mono = MonoClone(p->arr + i);
                InsertMonoToPoly(&result, &my_mono);
                i++;
            } else if (exp_j < exp_i) {
                Mono my_mono = MonoClone(q->arr + j);
                InsertMonoToPoly(&result, &my_mono);
                j++;
            } else {
                Poly add_result = PolyAdd(&p->arr[i].p, &q->arr[j].p);

                if (!PolyIsZero(&add_result)) {
                    Mono to_add = MonoFromPoly(&add_result, MonoGetExp(p->arr + i));
                    InsertMonoToPoly(&result, &to_add);
                }
                i++;
                j++;
            }
        }

        while (i < p->size) {
            Mono my_mono = MonoClone(p->arr + i);
            InsertMonoToPoly(&result, &my_mono);
            i++;
        }

        while (j < q->size) {
            Mono my_mono = MonoClone(q->arr + j);
            InsertMonoToPoly(&result, &my_mono);
            j++;
        }
    }
    Simplify(&result);

    return result;
}


Poly PolyAddMonos(size_t count, const Mono *monos) {
    if (count == 0) {
        return PolyZero();
    }

    Poly result = PolyZero();
    result.size = 0;
    Mono *monos_copy = malloc(count * sizeof(Mono));
    check_ptr(monos_copy);

    for (size_t i = 0; i < count; i++) {
        monos_copy[i] = monos[i];
    }
    qsort(monos_copy, count, sizeof(Mono), ComparatorExponents);

    size_t i = 0;

    while (i < count) {
        Poly current_sum = PolyClone(&monos_copy[i].p);
        size_t j = i + 1;
        while (j < count && MonoGetExp(monos_copy + j) == MonoGetExp(monos_copy + i)) {
            Poly temp_poly = PolyAdd(&current_sum, &monos_copy[j].p);
            PolyDestroy(&current_sum);
            current_sum = temp_poly;
            j++;
        }
        Simplify(&current_sum);
        if (!PolyIsZero(&current_sum)) {
            Mono new_mono = MonoFromPoly(&current_sum, MonoGetExp(monos_copy + i));
            InsertMonoToPoly(&result, &new_mono);
        }
        i = j;
    }

    for (size_t j = 0; j < count; j++) {
        MonoDestroy(&monos_copy[j]);
    }

    free(monos_copy);
    Simplify(&result);

    return result;
}

/**
 * Mnoży wielomian przez wielomian stały (liczbę).
 * @param[in] p : Wielomian którego jednomiany są do przemnożenia (nie-stały).
 * @param[in] q : Wielomian stały (liczba).
 * @return Wielomian będący wynikiem mnożenia.
 */
static Poly PolyCoeffMul(const Poly *p, const Poly *q) {
    Poly result;

    if (q->coeff == 0) {
        return PolyZero();
    }

    Mono *tmp = malloc((p->size) * sizeof(Mono));
    check_ptr(tmp);

    size_t amount_non_zero = 0;

    for (size_t j = 0; j < p->size; j++) {
        Poly mul_result = PolyMul(&p->arr[j].p, q);
        if (!PolyIsZero(&mul_result)) {
            tmp[amount_non_zero] = MonoFromPoly(&mul_result, p->arr[j].exp);
            amount_non_zero++;
        }
    }

    if (amount_non_zero == 0) {
        free(tmp);
        return PolyZero();
    }

    result = PolyAddMonos(amount_non_zero, tmp);
    free(tmp);
    Simplify(&result);

    return result;
}

Poly PolyMul(const Poly *p, const Poly *q) {
    Poly result;

    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        result.arr = NULL;
        result.coeff = p->coeff * q->coeff;
    } else if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        return PolyCoeffMul(q, p);
    } else if (!PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return PolyCoeffMul(p, q);
    } else {
        Mono *tmp = malloc((p->size) * (q->size) * sizeof(Mono));
        check_ptr(tmp);

        size_t amount_non_zero = 0;

        for (size_t i = 0; i < p->size; i++) {
            for (size_t j = 0; j < q->size; j++) {
                Poly multiplication_result = PolyMul(&(p->arr[i].p), &(q->arr[j]).p);
                poly_exp_t exponent = MonoGetExp(p->arr + i) + MonoGetExp(q->arr + j);
                if (!PolyIsZero(&multiplication_result)) {
                    tmp[amount_non_zero] = MonoFromPoly(&multiplication_result, exponent);
                    amount_non_zero++;
                }
            }
        }

        result = PolyAddMonos(amount_non_zero, tmp);
        free(tmp);
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
        if (p->size == 0) {
            return PolyFromCoeff(-p->coeff);
        }

        result.arr = malloc(sizeof(Mono) * (p->size));
        check_ptr(result.arr);
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
    Simplify(&sum);

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

/**
 * Wyznacza stopień podanego jednomianu.
 * @param[in] m : Jednomian którego stopień należy wyznaczyć.
 * @return Stopień jednomianu.
 */
static poly_exp_t MonoDeg(Mono *m) {
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

/**
 * Podnosi liczbę do zadanej potęgi.
 * @param[in] x : Podstawa operacji potęgowania.
 * @param[in] exp : Wykładnik operacji potęgowania.
 * @return Wynik potęgowania.
 */
static poly_coeff_t Power(poly_coeff_t x, poly_exp_t exp) {
    poly_coeff_t result = 1;

    for (poly_exp_t i = 0; i < exp; i++) {
        result *= x;
    }
    return result;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    Poly result = PolyZero();

    if (PolyIsCoeff(p)) {
        result = PolyFromCoeff(p->coeff);
        return result;
    }

    for (size_t i = 0; i < p->size; i++) {
        poly_exp_t my_exponent = p->arr[i].exp;
        poly_coeff_t constant = Power(x, my_exponent);
        Poly multiplier = PolyFromCoeff(constant);
        Poly current = PolyMul(&multiplier, &p->arr[i].p);
        Poly temp_result = PolyAdd(&result, &current);
        PolyDestroy(&current);
        PolyDestroy(&result);
        result = temp_result;
    }
    Simplify(&result);

    return result;
}




