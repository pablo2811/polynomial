#include <stdbool.h>
#include "poly.h"
#include "stack.h"

#ifndef POLYNOMIALS_PARSER_H
#define POLYNOMIALS_PARSER_H

#endif //POLYNOMIALS_PARSER_H


Poly ParsePoly(char **line, bool *err);

void RunCommand(Stack *s, char *line, int lineNumber);