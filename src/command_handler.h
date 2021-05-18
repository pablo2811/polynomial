/** @file
  Interfejs poleceń kalkulatora wielomanów.

  @author Paweł Fijałkowski <pf429189@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/


#ifndef POLYNOMIALS_COMMAND_HANDLER_C_H
#define POLYNOMIALS_COMMAND_HANDLER_C_H

#endif //POLYNOMIALS_COMMAND_HANDLER_C_H

/**
 * Funkcja realizująca w kalkulatorze polecenie ZERO
 * (dodanie wielomianu zerowego na stos s).
 * @param s: Stack
 */
void Zero(Stack *s);

/**
 * Funkcja realizująca w kalkulatorze polecenie IS_COEFF
 * (sprawdzenie czy wielomian na wierzchu stosu jest wspołczynnikiem).
 * @param s: Stack
 * @param err: bool
 */
void IsCoefficient(Stack *s, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie IS_ZERO
 * (sprawdzenie czy wielomian na wierzchu stosu jest zerowy).
 * @param s: Stack
 * @param err: bool
 */
void IsZero(Stack *s, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie CLONE
 * (wstawienie na stos kopii wielomianu z wierzchołka).
 * @param s: Stack
 * @param err: bool
 */
void Clone(Stack *s, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie ADD
 * (zapis na wierchołek stosu sumy dwóch wielomianów na jego wierzchu, usunięcie elementów sumy).
 * @param s: Stack
 * @param err: bool
 */
void Add(Stack *s, bool *err);

/**
  * Funkcja realizująca w kalkulatorze polecenie MUL
 * (zapis na wierchołek stosu iloczynu dwóch wielomianów na jego wierzchu, usunięcie elementów iloczynu).
 * @param s: Stack
 * @param err: bool
 */
void Mul(Stack *s, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie NEG
 * (zapis na miejsce wierzchołka negacji).
 * @param s: Stack
 * @param err: bool
 */
void Neg(Stack *s, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie SUB
 * (zapis na wierchołek stosu różnicy dwóch wielomianów na jego wierzchu, usunięcie elementów).
 * @param s: Stack
 * @param err: bool
 */
void Sub(Stack *s, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie IS_EQ
 * (sprawdzenie czy wielomiany na wierzchu stosu są równe).
 * @param s: Stack
 * @param err: bool
 */
void IsEq(Stack *s, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie DEG
 * (wypisuje na standardowe wyjście stopień wielomianu na wierzchu stosu).
 * @param s: Stack
 * @param err: bool
 */
void Deg(Stack *s, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie DEG_BY
 * (wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną `idx`).
 * @param s: Stack
 * @param x: unsigned long long
 * @param err: bool
 */
void DegBy(Stack *s, unsigned long long x, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie PRINT
 * (wypisuje na standardowe wyjście wielomian z wierzchołka stosu).
 * @param s: Stack
 * @param err: bool
 */
void Print(Stack *s, bool *err);

/**
 * Funkcja realizująca w kalkulatorze polecenie POP
 * (usuwa wielomian z wierzchołka stosu).
 * @param s: Stack
 * @param err: bool
 */
void Pop(Stack *s, bool *err);


/**
 * Funkcja realizująca w kalkulatorze polecenie AT
 * (wylicza wartość wielomianu w punkcie x, usuwa wielomian z wierzchołka i wstawia na stos wynik operacji).
 * @param s: Stack
 * @param err: bool
 */
void At(Stack *s, long long x, bool *err);