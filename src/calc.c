#define  _GNU_SOURCE

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

#define COMMENT_SIGN '#'

static void StackKill(Stack myStack) {

    for (int i = 0; i < myStack.amount; i++) {
        PolyDestroy(myStack.stack + i);
    }

    free(myStack.stack);
}

void Run() {
    char *buffer = NULL;
    size_t bufferSize = 0;
    ssize_t characters = getline(&buffer, &bufferSize, stdin);
    int lineNumber = 0;
    Stack myStack = GetEmptyStack();

    while (characters != -1) {
        bool err = false;
        char *ptr = buffer;
        lineNumber++;

        if (characters <= 1 || (characters > 0 && *buffer == COMMENT_SIGN)) {
            characters = getline(&buffer, &bufferSize, stdin);
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
        characters = getline(&buffer, &bufferSize, stdin);
    }

    free(buffer);
    StackKill(myStack);
}


int main() {
    Run();

    return 0;
}





