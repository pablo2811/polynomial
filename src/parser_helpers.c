/** @file
  Implementacja funkcji pomocnicznych potrzebnych w procesie parsowania.

  @author Paweł Fijałkowski <pf429189@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>

long GetCoefficient(char **line, bool *pBoolean) {
    if (**line == '+') {
        *pBoolean = false;
        return 0;
    }

    char *endPtr;
    long argument = strtol(*line, &endPtr, 10);

    if (endPtr == *line || ((argument == LONG_MAX || argument == LONG_MIN) && errno == ERANGE)) {
        *pBoolean = false;
    } else {
        *pBoolean = true;
        *line = endPtr;
        return argument;
    }

    return 0;
}

void SimpleCheck(const char *line, bool *err) {
    bool isCoefficient;
    char *copy = malloc((strlen(line) + 1) * (sizeof(char)));
    char *copyBeg = copy;
    strcpy(copy, line);
    GetCoefficient(&copy, &isCoefficient);

    if ((isCoefficient && *copy != '\n') || (!isCoefficient && *copy != '(')) {
        *err = true;
    }

    free(copyBeg);
}

void AdvancedCheck(const char *line, bool *err) {
    char *goodChars = "(),+-";
    int openBrackets, closedBrackets;
    openBrackets = closedBrackets = 0;

    while (*line != '\n') {
        if (strchr(goodChars, *line) == NULL && !isdigit(*line)) {
            *err = true;
            break;
        }
        openBrackets += (*line == '(');
        closedBrackets += (*line == ')');
        if (openBrackets < closedBrackets) {
            *err = true;
            break;
        }
        line++;
    }

    if (openBrackets != closedBrackets) {
        *err = true;
    }
}

bool EachSignNumerical(const char *string) {
    while (*string != '\0' && *string != '\n') {
        if (*string - '0' > 10 || *string - '0' < 0) {
            return false;
        }
        string++;
    }

    return true;
}

bool StartsWith(const char *str, const char *pre) {
    size_t lenPre = strlen(pre), lenStr = strlen(str);

    return lenStr < lenPre ? false : memcmp(pre, str, lenPre) == 0;
}


int GetExponent(char **line, bool *isExponent) {
    if (**line == '+') {
        *isExponent = false;
        return 0;
    }

    char *endPtr;
    long argument = strtol(*line, &endPtr, 10);

    if (endPtr == *line || (argument == LONG_MAX && errno == ERANGE) || (argument < 0 || argument >= INT_MAX)) {
        *isExponent = false;
    } else {
        *line = endPtr;
        *isExponent = true;
        return (int) argument;
    }

    return 0;
}
