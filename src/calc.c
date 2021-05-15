#define  _GNU_SOURCE

#include <stdio.h>
#include <stddef.h>
#include "parser.h"

#define COMMENT_SIGN '#'

void run() {
    char *buffer;
    size_t bufsize = 0;
    ssize_t characters;
    int lineNumber = 1;
    Stack myStack = getEmptyStack();
    while ((characters = getline(&buffer, &bufsize, stdin)) != -1) {
        bool err = false;
        if (characters <= 1 || (characters > 0 && *buffer == COMMENT_SIGN)) continue;
        if ((90 >= *buffer && *buffer >= 65) || (122 >= *buffer && *buffer >= 97)) {
            runCommand(&myStack, buffer, (int) characters);
        } else {
            Poly parsed = parsePoly(&buffer, &err);
            if (err) {
                printf("ERROR %d WRONG POLY\n", lineNumber);
            } else {
                pushStack(&myStack, &parsed);
            }
        }
        lineNumber++;
    }

}


int main() {
    run();
    return 0;
}





