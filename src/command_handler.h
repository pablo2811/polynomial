#ifndef POLYNOMIALS_COMMAND_HANDLER_C_H
#define POLYNOMIALS_COMMAND_HANDLER_C_H

#endif //POLYNOMIALS_COMMAND_HANDLER_C_H

void zero(Stack *s);

void isCoeff(Stack *s, bool *err);

void isZero(Stack *s, bool *err);

void clone(Stack *s, bool *err);

void add(Stack *s, bool *err);

void mul(Stack *s, bool *err);

void neg(Stack *s, bool *err);

void sub(Stack *s, bool *err);

void isEq(Stack *s, bool *err);

void deg(Stack *s, bool *err);

void print(Stack *s, bool *err);

void pop(Stack *s, bool *err);

void degBy(Stack *s, unsigned long long x, bool *err);

void at(Stack *s, long long x, bool *err);