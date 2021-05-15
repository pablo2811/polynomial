#ifndef POLYNOMIALS_COMMAND_HANDLER_C_H
#define POLYNOMIALS_COMMAND_HANDLER_C_H

#endif //POLYNOMIALS_COMMAND_HANDLER_C_H

bool zero(Stack *s);

bool isCoeff(Stack *s, bool *err);

bool isZero(Stack *s, bool *err);

bool clone(Stack *s, bool *err);

bool add(Stack *s, bool *err);

bool mul(Stack *s, bool *err);

bool neg(Stack *s, bool *err);

bool sub(Stack *s, bool *err);

bool isEq(Stack *s, bool *err);

bool deg(Stack *s, bool *err);

bool print(Stack *s, bool *err);

bool pop(Stack *s, bool *err);

bool degBy(Stack *s, unsigned long long x, bool *err);

bool at(Stack *s, long long x, bool *err);