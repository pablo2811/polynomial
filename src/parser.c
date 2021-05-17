#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>

#include "parser.h"
#include "command_handler.h"

long GetCoeff(char **line, bool *isCoeff) {
    if (**line == '+') {
        *isCoeff = false;
        return 0;
    }

    char *endptr;
    long argument = strtol(*line, &endptr, 10);

    if (endptr == *line || ((argument == LONG_MAX || argument == LONG_MIN) && errno == ERANGE)) {
        *isCoeff = false;
    } else {
        *isCoeff = true;
        *line = endptr;
        return argument;
    }

    return 0;
}

int GetExponent(char **line, bool *isExponent) {
    if (**line == '+') {
        *isExponent = false;
        return 0;
    }

    char *endptr;
    long argument = strtol(*line, &endptr, 10);

    if (endptr == *line || (argument == LONG_MAX && errno == ERANGE) || (argument < 0 || argument >= INT_MAX)) {
        *isExponent = false;
    } else {
        *line = endptr;
        *isExponent = true;
        return (int) argument;
    }

    return 0;
}

bool EachSignNumerical(const char *string) {
    while (*string != '\0' && *string != '\n') {
        if (*string - '0' > 10 || *string - '0' < 0) {
            return false;
        }
        string++;
    }

    return true;
}

bool StartsWith(const char *str, const char *pre) {
    size_t lenpre = strlen(pre), lenstr = strlen(str);

    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

void SimpleCheck(const char *line, bool *err) {
    bool isCoeff;
    char *copy = malloc((strlen(line) + 1) * (sizeof(char)));
    char *copyBeg = copy;
    strcpy(copy, line);
    GetCoeff(&copy, &isCoeff);

    if ((isCoeff && *copy != '\n') || (!isCoeff && *copy != '(')) {
        *err = true;
    }

    free(copyBeg);
}

void AdvancedCheck(const char *line, bool *err) {
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


Poly ParsePolyUtil(char **line, bool *err) {
    bool isCoeff;
    long long coeff = GetCoeff(line, &isCoeff);
    Poly temp = PolyZero();

    if (isCoeff) {
        return PolyFromCoeff(coeff);
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

Mono ParseMono(char **line, bool *err) {
    bool isExp;
    Poly fooPoly = PolyZero();

    if (**line == '(') {

        (*line)++;
        Poly coeff = ParsePolyUtil(line, err);
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
        return (Mono) {.p = coeff, .exp = exp};
    } else {
        *err = true;
        return MonoFromPoly(&fooPoly, 0);
    }
}

void RunCommand(Stack *s, char *line, int lineNumber) {
    bool err = false;
    char *endptr, *argumentString;

    if (StartsWith(line, "AT")) {
        argumentString = line + 2;
        long long argument = strtoll(argumentString, &endptr, 10);

        if (*argumentString != ' ' || endptr == argumentString ||
            ((argument == LLONG_MAX || argument == LLONG_MIN) && errno == ERANGE) ||
            !EachSignNumerical(argumentString + 1)) {
            fprintf(stderr, "ERROR %d AT WRONG VALUE\n", lineNumber);
        } else {
            At(s, argument, &err);
        }

    } else if (StartsWith(line, "DEG_BY")) {
        argumentString = line + 7;
        long argument = strtol(argumentString, &endptr, 10);

        if (endptr == argumentString || (argument == LONG_MAX && errno == ERANGE) ||
            !EachSignNumerical(argumentString)) {
            fprintf(stderr, "ERROR %d DEG BY WRONG VARIABLE\n", lineNumber);
        } else {
            DegBy(s, argument, &err);
        }

    } else if (strcmp(line, "ZERO\n") == 0) {
        Zero(s);
    } else if (strcmp(line, "IS_COEFF\n") == 0) {
        IsCoeff(s, &err);
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