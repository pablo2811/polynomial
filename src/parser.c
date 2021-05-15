#include "parser.h"
#include "stack.h"
#include "command_handler.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

Poly parsePoly(char *line, int length, bool *err) {
    // todo
    Poly result;
    return result;
}

Mono parseMono(char *line, int length, bool *err) {
    // todo
    Mono result;
    return result;
}

void runCommand(Stack *s, char *line, int length, int lineNumber) {
    char *space = strchr(line, ' ');
    bool err;
    if (space == NULL) {
        if (strcmp(line, "ZERO\n") == 0) {
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
            printf("ERROR %d WRONG COMMAND\n", lineNumber);
        }
        if (err) printf("ERROR %d STACK UNDERFLOW\n", lineNumber);
    } else {
        char *endptr, *argumentString;
        if (strstr(line, "AT ") != NULL) {
            argumentString = line + 3;
            long long argument = strtoll(argumentString, &endptr, 10);
            if (endptr == argumentString || ((argument == LLONG_MAX || argument == LLONG_MIN) && errno == ERANGE)) {
                printf("ERROR %d AT WRONG VALUE\n", lineNumber);
            } else {
                at(s, argument, &err);
            }
        } else if (strstr(line, "DEG_BY ") != NULL) {
            argumentString = line + 7;
            unsigned long long argument = strtoull(argumentString, &endptr, 10);
            if (endptr == argumentString || (argument == ULLONG_MAX && errno == ERANGE)) {
                printf("ERROR %d DEG BY WRONG VARIABLE\n", lineNumber);
            } else {
                degBy(s, argument, &err);
            }
        } else {
            printf("ERROR %d WRONG COMMAND\n", lineNumber);
        }
        if (err) printf("ERROR %d STACK UNDERFLOW\n", lineNumber);
    }
}