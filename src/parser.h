#include <stdbool.h>
#include "poly.h"

#ifndef POLYNOMIALS_PARSER_H
#define POLYNOMIALS_PARSER_H

#endif //POLYNOMIALS_PARSER_H

Poly parsePoly(char *line, int length, bool *err);

Mono parseMono(char *line, int length, bool *err);

void runCommand(Stack *s, char *line, int length);