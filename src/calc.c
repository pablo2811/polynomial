#define  _GNU_SOURCE

#include <stdio.h>
#include <stddef.h>
#include "parser.h"

#define COMMENT_SIGN '#'

void run() {
    char *buffer;
    size_t bufsize = 0;
    ssize_t characters;
    int lineNumber = 0;
    Stack myStack = getEmptyStack();
    while ((characters = getline(&buffer, &bufsize, stdin)) != -1) {
        bool err = false;
        lineNumber++;
        if (characters <= 1 || (characters > 0 && *buffer == COMMENT_SIGN)) continue;
        if ((90 >= *buffer && *buffer >= 65) || (122 >= *buffer && *buffer >= 97)) {
            runCommand(&myStack, buffer, lineNumber);
        } else {
            Poly parsed = parsePoly(&buffer, &err);
            if (err) {
                fprintf(stderr, "ERROR %d WRONG POLY\n", lineNumber);
            } else {
                pushStack(&myStack, &parsed);
            }
        }

    }
}


int main() {
    run();
    return 0;
}





