#include "Notecard.api.v1"

J * JGetObjectItemResult;
Jbool JHasObjectItemResult;
Jbool JIsNumberResult;
JNUMBER JNumberValueResult;

J * JGetObjectItem(const J * const, const char * const)
{
    return JGetObjectItemResult;
}

Jbool JHasObjectItem(const J *, const char *)
{
    return JHasObjectItemResult;
}

Jbool JIsNumber(const J * const)
{
    return JIsNumberResult;
}

JNUMBER JNumberValue(J *)
{
    return JNumberValueResult;
}
