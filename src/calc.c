#define  _GNU_SOURCE

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

#define COMMENT_SIGN '#'

void StackKill(Stack myStack) {

    for (int i = 0; i < myStack.amount; i++) {
        PolyDestroy(myStack.stack + i);
    }
    free(myStack.stack);
}

void Run() {
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t characters;
    int lineNumber = 0;
    Stack myStack = GetEmptyStack();
    while ((characters = getline(&buffer, &bufsize, stdin)) != -1) {
        bool err = false;
        char *ptr = buffer;
        lineNumber++;

        if (characters <= 1 || (characters > 0 && *buffer == COMMENT_SIGN)) {
            continue;
        }
        if (isalpha(*buffer)) {
            RunCommand(&myStack, buffer, lineNumber);
        } else {
            Poly parsed = ParsePoly(&buffer, &err);
            if (err) {
                fprintf(stderr, "ERROR %d WRONG POLY\n", lineNumber);
                PolyDestroy(&parsed);
            } else {
                PushStack(&myStack, &parsed);
            }
        }
        free(ptr);
        buffer = NULL;
    }

    free(buffer);
    StackKill(myStack);
}


int main() {
    Run();
    return 0;
}





