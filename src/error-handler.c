//
// Created by jackhammer on 03.05.24.
//

#include "error-handler.h"

void fatal_error(const char* message, const int code) {
    fprintf(stderr, "%s\n", message);
    exit(code);
}