#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "parser.h"
#include "command_handler.h"

long getCoeff(char **line, bool *isCoeff) {
    char *endptr;
    long long argument = strtol(*line, &endptr, 10);
    if (endptr == *line || ((argument == LONG_MAX || argument == LONG_MIN) && errno == ERANGE)) {
        *isCoeff = false;
    } else {
        *isCoeff = true;
        *line = endptr;
        return argument;
    }
    return 0;
}

long getExponent(char **line, bool *isExponent) {
    char *endptr;
    long argument = strtol(*line, &endptr, 10);
    if (endptr == *line || (argument == LONG_MAX && errno == ERANGE)) {
        *isExponent = false;
    } else {
        *line = endptr;
        *isExponent = true;
        return argument;
    }
    return 0;
}

bool eachSignNumerical(const char *string) {
    while (*string != '\0' && *string != '\n') {
        if (*string - '0' > 10 || *string - '0' < 0) {
            return false;
        }
        string++;
    }
    return true;
}

bool startsWith(const char *str, const char *pre) {
    size_t lenpre = strlen(pre), lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}


Poly parsePoly(char **line, bool *err) {
    bool isCoeff;
    long long coeff = getCoeff(line, &isCoeff);
    if (isCoeff) return PolyFromCoeff(coeff);
    Poly temp = PolyZero();
    while (**line != ',' && **line != '\n' && !*err) {
        Mono current;
        switch (**line) {
            case '(':
                current = parseMono(line, err);
                insertMonoToPoly(&temp, &current);
                break;
            case '+':
                (*line)++;
                break;
            default:
                *err = true;

        }
    }

    return PolyAddMonos(temp.size, temp.arr);
}

Mono parseMono(char **line, bool *err) {
    bool isExp;
    if (**line == '(') {
        (*line)++;
        Poly coeff = parsePoly(line, err);
        if (**line != ',') *err = true;
        (*line)++;
        long exp = getExponent(line, &isExp);
        if (**line != ')' || !isExp) *err = true;
        (*line)++;
        return (Mono) {.p = coeff, .exp = (int) exp};
    } else {
        *err = true;
        Poly fooPoly = PolyZero();
        return MonoFromPoly(&fooPoly, 1);
    }
}

void runCommand(Stack *s, char *line, int lineNumber) {
    bool err = false;
    char *endptr, *argumentString;
    if (startsWith(line, "AT")) {
        argumentString = line + 2;
        long long argument = strtoll(argumentString, &endptr, 10);
        if (*argumentString != ' ' || endptr == argumentString ||
            ((argument == LLONG_MAX || argument == LLONG_MIN) && errno == ERANGE) ||
            !eachSignNumerical(argumentString + 1)) {
            fprintf(stderr, "ERROR %d AT WRONG VALUE\n", lineNumber);
        } else {
            at(s, argument, &err);
        }
    } else if (startsWith(line, "DEG_BY")) {
        argumentString = line + 7;
        unsigned long long argument = strtoull(argumentString, &endptr, 10);
        if (endptr == argumentString || (argument == ULLONG_MAX && errno == ERANGE) ||
            !eachSignNumerical(argumentString)) {
            fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", lineNumber);
        } else {
            degBy(s, argument, &err);
        }
    } else if (strcmp(line, "ZERO\n") == 0) {
        zero(s);
    } else if (strcmp(line, "IS_COEFF\n") == 0) {
        isCoeff(s, &err);
    } else if (strcmp(line, "IS_ZERO\n") == 0) {
        isZero(s, &err);
    } else if (strcmp(line, "CLONE\n") == 0) {
        clone(s, &err);
    } else if (strcmp(line, "ADD\n") == 0) {
        add(s, &err);
    } else if (strcmp(line, "MUL\n") == 0) {
        mul(s, &err);
    } else if (strcmp(line, "NEG\n") == 0) {
        neg(s, &err);
    } else if (strcmp(line, "SUB\n") == 0) {
        sub(s, &err);
    } else if (strcmp(line, "IS_EQ\n") == 0) {
        isEq(s, &err);
    } else if (strcmp(line, "DEG\n") == 0) {
        deg(s, &err);
    } else if (strcmp(line, "PRINT\n") == 0) {
        print(s, &err);
    } else if (strcmp(line, "POP\n") == 0) {
        pop(s, &err);
    } else {
        fprintf(stderr, "ERROR %d WRONG COMMAND\n", lineNumber);
    }
    if (err) fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", lineNumber);
}