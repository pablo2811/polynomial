/** @file
  Interfejs parsowania podanych z wiersza poleceń wielomianów i komend.

  @author Paweł Fijałkowski <pf429189@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "poly.h"
#include "stack.h"

#ifndef POLYNOMIALS_PARSER_H
#define POLYNOMIALS_PARSER_H

#endif //POLYNOMIALS_PARSER_H

/**
 * Funkcja parsująca wielomian z wejścia.
 * W przypadku błędu parsowania, ustawia err = true.
 * @param line - wskaźnik do linii zawierającej wielomian.
 * @param err - zmienna przechowująca fakt zaistnienia błędu.
 * @return wielomian będący wynikiem parsownia
 */
Poly ParsePoly(char **line, bool *err);

/**
 * Funkcja odpowiedzialna za czytanie poleceń z wejścia.
 * W przypadku błędnego polecenia, wypisuje na standardowe wyjście błędu odpowiedni komunikat.
 * @param s - Stack, zawierający aktualne wielomiany.
 * @param line - linia zawierająca treść polecenia.
 * @param lineNumber - numer aktualnej lini.
 */
void RunCommand(Stack *s, char *line, int lineNumber);