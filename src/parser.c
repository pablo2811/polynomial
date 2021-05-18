/** @file
  Implementacja parsowania podanych z wiersza poleceń wielomianów i komend.

  @author Paweł Fijałkowski <pf429189@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "command_handler.h"
#include "parser_helpers.h"

static Poly ParsePolyUtil(char **line, bool *err);

static Mono ParseMono(char **line, bool *err);

Poly ParsePoly(char **line, bool *err) {
    SimpleCheck(*line, err);
    AdvancedCheck(*line, err);

    if (*err) {
        return PolyZero();
    }

    return ParsePolyUtil(line, err);
}

void RunCommand(Stack *s, char *line, int lineNumber) {
    bool err = false;
    char *endPtr, *argumentString;

    if (StartsWith(line, "AT")) {
        argumentString = line + 2;
        errno = 0;
        long argument = strtol(argumentString, &endPtr, 10);

        if (*argumentString != ' ' || endPtr == argumentString || errno == ERANGE ||
            !EachSignNumerical(argumentString + 1)) {
            fprintf(stderr, "ERROR %d AT WRONG VALUE\n", lineNumber);
        } else {
            At(s, argument, &err);
        }

    } else if (StartsWith(line, "DEG_BY")) {
        argumentString = line + 7;
        errno = 0;
        unsigned long long argument = strtoull(argumentString, &endPtr, 10);

        if (*(argumentString - 1) != ' ' || endPtr == argumentString || errno == ERANGE ||
            !EachSignNumerical(argumentString)) {
            fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", lineNumber);
        } else {
            DegBy(s, argument, &err);
        }

    } else if (strcmp(line, "ZERO\n") == 0 || strcmp(line, "ZERO") == 0) {
        Zero(s);
    } else if (strcmp(line, "IS_COEFF\n") == 0 || strcmp(line, "IS_COEFF") == 0) {
        IsCoefficient(s, &err);
    } else if (strcmp(line, "IS_ZERO\n") == 0 || strcmp(line, "IS_ZERO") == 0) {
        IsZero(s, &err);
    } else if (strcmp(line, "CLONE\n") == 0 || strcmp(line, "CLONE") == 0) {
        Clone(s, &err);
    } else if (strcmp(line, "ADD\n") == 0 || strcmp(line, "ADD") == 0) {
        Add(s, &err);
    } else if (strcmp(line, "MUL\n") == 0 || strcmp(line, "MUL") == 0) {
        Mul(s, &err);
    } else if (strcmp(line, "NEG\n") == 0 || strcmp(line, "NEG") == 0) {
        Neg(s, &err);
    } else if (strcmp(line, "SUB\n") == 0 || strcmp(line, "SUB") == 0) {
        Sub(s, &err);
    } else if (strcmp(line, "IS_EQ\n") == 0 || strcmp(line, "IS_EQ") == 0) {
        IsEq(s, &err);
    } else if (strcmp(line, "DEG\n") == 0 || strcmp(line, "DEG") == 0) {
        Deg(s, &err);
    } else if (strcmp(line, "PRINT\n") == 0 || strcmp(line, "PRINT") == 0) {
        Print(s, &err);
    } else if (strcmp(line, "POP\n") == 0 || strcmp(line, "POP") == 0) {
        Pop(s, &err);
    } else {
        fprintf(stderr, "ERROR %d WRONG COMMAND\n", lineNumber);
    }
    if (err) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", lineNumber);
    }

}

/**
 * Funkcja pomocnicza wykonująca właściwe (po wstępnym sprawdzeniu poprawności w ParsePoly) obliczenia parsowania.
 * @param line - linia zawierająca wielomian.
 * @param err  - zmienna przechowyująca fakt zaistnienia błędu.
 * @return poly - sparsowany wielomian.
 */
static Poly ParsePolyUtil(char **line, bool *err) {
    bool isCoefficient;
    long long coefficient = GetCoefficient(line, &isCoefficient);
    Poly temp = PolyZero();

    if (isCoefficient) {
        return PolyFromCoeff(coefficient);
    }

    while (**line != ',' && **line != '\n' && !*err) {
        Mono current;

        if (**line == '(') {
            current = ParseMono(line, err);
            InsertMonoToPoly(&temp, &current);
            if ((**line != ',' && **line != '+' && **line != '\n') || (**line == '+' && *(*line + 1) == '\n')) {
                *err = true;
                PolyDestroy(&temp);
                return PolyZero();
            }
            if (**line != '\n' && **line != ',') {
                (*line)++;
            }
        } else {
            *err = true;
            PolyDestroy(&temp);
            return PolyZero();
        }
    }

    Poly result = PolyAddMonos(temp.size, temp.arr);
    free(temp.arr);

    return result;
}

/**
 * Funkcja wykonująca obliczenia na rzecz parsowania jednomianu.
 * @param line - linia zawierająca jednomian do zparsowania.
 * @param err - zmienna przechowyująca fakt zaistnienia błędu.
 * @return
 */
static Mono ParseMono(char **line, bool *err) {
    bool isExp;
    Poly fooPoly = PolyZero();

    if (**line == '(') {

        (*line)++;
        Poly coefficient = ParsePolyUtil(line, err);
        if (**line != ',') {
            *err = true;
            return MonoFromPoly(&fooPoly, 0);
        }

        (*line)++;
        int exp = GetExponent(line, &isExp);
        if (**line != ')' || !isExp) {
            *err = true;
            return MonoFromPoly(&fooPoly, 0);
        }

        (*line)++;
        return (Mono) {.p = coefficient, .exp = exp};
    } else {
        *err = true;
        return MonoFromPoly(&fooPoly, 0);
    }
}

