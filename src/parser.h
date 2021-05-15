#include <stdbool.h>
#include "poly.h"
#include "stack.h"

#ifndef POLYNOMIALS_PARSER_H
#define POLYNOMIALS_PARSER_H

#endif //POLYNOMIALS_PARSER_H

Poly parsePoly(char **line, bool *err);

Mono parseMono(char **line, bool *err);

void runCommand(Stack *s, char *line, int lineNumber);