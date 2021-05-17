#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>

#include "parser.h"
#include "command_handler.h"

static Poly ParsePolyUtil(char **line, bool *err);

static Mono ParseMono(char **line, bool *err);

static long GetCoefficient(char **line, bool *pBoolean) {
    if (**line == '+') {
        *pBoolean = false;
        return 0;
    }

    char *endPtr;
    long argument = strtol(*line, &endPtr, 10);

    if (endPtr == *line || ((argument == LONG_MAX || argument == LONG_MIN) && errno == ERANGE)) {
        *pBoolean = false;
    } else {
        *pBoolean = true;
        *line = endPtr;
        return argument;
    }

    return 0;
}

static int GetExponent(char **line, bool *isExponent) {
    if (**line == '+') {
        *isExponent = false;
        return 0;
    }

    char *endPtr;
    long argument = strtol(*line, &endPtr, 10);

    if (endPtr == *line || (argument == LONG_MAX && errno == ERANGE) || (argument < 0 || argument >= INT_MAX)) {
        *isExponent = false;
    } else {
        *line = endPtr;
        *isExponent = true;
        return (int) argument;
    }

    return 0;
}

static bool EachSignNumerical(const char *string) {
    while (*string != '\0' && *string != '\n') {
        if (*string - '0' > 10 || *string - '0' < 0) {
            return false;
        }
        string++;
    }

    return true;
}

static bool StartsWith(const char *str, const char *pre) {
    size_t lenPre = strlen(pre), lenStr = strlen(str);

    return lenStr < lenPre ? false : memcmp(pre, str, lenPre) == 0;
}

static void SimpleCheck(const char *line, bool *err) {
    bool isCoefficient;
    char *copy = malloc((strlen(line) + 1) * (sizeof(char)));
    char *copyBeg = copy;
    strcpy(copy, line);
    GetCoefficient(&copy, &isCoefficient);

    if ((isCoefficient && *copy != '\n') || (!isCoefficient && *copy != '(')) {
        *err = true;
    }

    free(copyBeg);
}

static void AdvancedCheck(const char *line, bool *err) {
    char *goodChars = "(),+-";
    int openBrackets, closedBrackets;
    openBrackets = closedBrackets = 0;

    while (*line != '\n') {
        if (strchr(goodChars, *line) == NULL && !isdigit(*line)) {
            *err = true;
            break;
        }
        openBrackets += (*line == '(');
        closedBrackets += (*line == ')');
        if (openBrackets < closedBrackets) {
            *err = true;
            break;
        }
        line++;
    }

    if (openBrackets != closedBrackets) {
        *err = true;
    }
}

Poly ParsePoly(char **line, bool *err) {
    SimpleCheck(*line, err);
    AdvancedCheck(*line, err);

    if (*err) {
        return PolyZero();
    }

    return ParsePolyUtil(line, err);
}


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

void RunCommand(Stack *s, char *line, int lineNumber) {
    bool err = false;
    char *endPtr, *argumentString;

    if (StartsWith(line, "AT")) {
        argumentString = line + 2;
        long long argument = strtoll(argumentString, &endPtr, 10);

        if (*argumentString != ' ' || endPtr == argumentString ||
            ((argument == LLONG_MAX || argument == LLONG_MIN) && errno == ERANGE) ||
            !EachSignNumerical(argumentString + 1)) {
            fprintf(stderr, "ERROR %d AT WRONG VALUE\n", lineNumber);
        } else {
            At(s, argument, &err);
        }

    } else if (StartsWith(line, "DEG_BY")) {
        argumentString = line + 7;
        long argument = strtol(argumentString, &endPtr, 10);

        if (endPtr == argumentString || (argument == LONG_MAX && errno == ERANGE) ||
            !EachSignNumerical(argumentString)) {
            fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", lineNumber);
        } else {
            DegBy(s, argument, &err);
        }

    } else if (strcmp(line, "ZERO\n") == 0) {
        Zero(s);
    } else if (strcmp(line, "IS_COEFF\n") == 0) {
        IsCoefficient(s, &err);
    } else if (strcmp(line, "IS_ZERO\n") == 0) {
        IsZero(s, &err);
    } else if (strcmp(line, "CLONE\n") == 0) {
        Clone(s, &err);
    } else if (strcmp(line, "ADD\n") == 0) {
        Add(s, &err);
    } else if (strcmp(line, "MUL\n") == 0) {
        Mul(s, &err);
    } else if (strcmp(line, "NEG\n") == 0) {
        Neg(s, &err);
    } else if (strcmp(line, "SUB\n") == 0) {
        Sub(s, &err);
    } else if (strcmp(line, "IS_EQ\n") == 0) {
        IsEq(s, &err);
    } else if (strcmp(line, "DEG\n") == 0) {
        Deg(s, &err);
    } else if (strcmp(line, "PRINT\n") == 0) {
        Print(s, &err);
    } else if (strcmp(line, "POP\n") == 0) {
        Pop(s, &err);
    } else {
        fprintf(stderr, "ERROR %d WRONG COMMAND\n", lineNumber);
    }
    if (err) {
        fprintf(stderr, "ERROR %d STACK UNDERFLOW\n", lineNumber);
    }

}