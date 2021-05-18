
#ifndef POLYNOMIALS_PARSER_HELPERS_H
#define POLYNOMIALS_PARSER_HELPERS_H

#endif //POLYNOMIALS_PARSER_HELPERS_H

/**
 * Funkcja wykonująca proste sprawdzenie poprawności wielomianu do zparsowania.
 * Ustawia err = true, jeśli wielomian nie spełnia odpowednich warunków.
 * @param line - linia zawieraja wielomian.
 * @param err - zmienna przechowywująca informację o błędzie.
 */
void SimpleCheck(const char *line, bool *err);

/**
 * Funkcja wykonująca złożone sprawdzenie poprawności wielomianu do zparsowania.
 * Ustawia err = true, jeśli wielomian nie spełnia odpowednich warunków.
 * @param line - linia zawieraja wielomian.
 * @param err - zmienna przechowywująca informację o błędzie.
 */
void AdvancedCheck(const char *line, bool *err);

/**
 * Funkcja pomocnicza sprawdzająca czy każdy element ciągu znakowego jest cyfrą.
 * @param string - ciąg znaków.
 * @return czy każdy znak jest cyfrą?
 */
bool EachSignNumerical(const char *string);

/**
 * Funkcja zwracająca znajdujący się na początku `*line` współczynnik wielomianu.
 * Przesuwa wskaźnik na najbliższy nie liczbowy znak.
 * Ustawia flagę pBoolean = true, jeśli pobrana wartość jest poprawnym współczynnikiem.
 * @param line - linia, której początek zawiera współczynnik.
 * @param pBoolean - sukces/porażka.
 * @return współczynnik wielomianu.
 */
long GetCoefficient(char **line, bool *pBoolean);

/**
 * Funkcja zwracająca znajdujący się na początku `*line` wykładnik wielomianu.
 * Przesuwa wskaźnik na najbliższy nie liczbowy znak.
 * Ustawia flagę isExponent = true, jeśli pobrana wartość jest poprawnym wykładnikiem.
 * @param line - linia, której początek zawiera współczynnik.
 * @param isExponent - sukces/porażka.
 * @return wykładnik wielomianu.
 */
int GetExponent(char **line, bool *isExponent);

/**
 * Funkcja pomocnicza sprawdzająca czy ciąg znaków `str` ma prefix `pre.
 * @param str - rozważany ciąg znaków.
 * @param pre - szukany prefix.
 * @return czy pre jest prefixem str?
 */
bool StartsWith(const char *str, const char *pre);
