#ifndef POLYNOMIALS_COMMAND_HANDLER_C_H
#define POLYNOMIALS_COMMAND_HANDLER_C_H

#endif //POLYNOMIALS_COMMAND_HANDLER_C_H

void Zero(Stack *s);

void IsCoefficient(Stack *s, bool *err);

void IsZero(Stack *s, bool *err);

void Clone(Stack *s, bool *err);

void Add(Stack *s, bool *err);

void Mul(Stack *s, bool *err);

void Neg(Stack *s, bool *err);

void Sub(Stack *s, bool *err);

void IsEq(Stack *s, bool *err);

void Deg(Stack *s, bool *err);

void Print(Stack *s, bool *err);

void Pop(Stack *s, bool *err);

void DegBy(Stack *s, unsigned long long x, bool *err);

void At(Stack *s, long long x, bool *err);