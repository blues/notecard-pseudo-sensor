#include "stdlib.mock.hpp"

Rand_Parameters rand_Parameters;

int rand (void) {
    return rand_Parameters.result;
}
