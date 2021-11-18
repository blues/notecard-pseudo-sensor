#include <stddef.h>

struct Rand_Parameters {
    Rand_Parameters (
        void
    ) :
        invoked(0),
        result(0)
    { }
    void
    reset (
        void
    ) {
        invoked = 0;
        result = 0;
    }
    size_t invoked;
    int result;
};

int rand (void);

extern Rand_Parameters rand_Parameters;
