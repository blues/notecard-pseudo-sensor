#include "NotecardPseudoSensor.h"

#ifndef MOCK
  #include <stdlib.h>
  #include <Notecard.h>
#else
  #include "mock/Notecard.api.v1"
  #include "mock/stdlib.mock.hpp"
#endif

using namespace blues;

NotecardPseudoSensor::NotecardPseudoSensor (
    Notecard & notecard_
) :
    _notecard(notecard_)
{

}

float
NotecardPseudoSensor::humidity (
    void
) {
    static const float HUMIDITY_FLOOR = 45;

    /*
     * Create a uniform offset of [0,5)%RH, with 4 decimal places
     *
     * NOTE: `random()` has not been seeded (`srand()`) as there
     *        is no need of true randomness in this application.
     */
    const float RANDOM_OFFSET = ((rand() % 50000) / 10000.0f);

    return (HUMIDITY_FLOOR + RANDOM_OFFSET);
}

float
NotecardPseudoSensor::temp (
    void
) {
    float result = 0.0f;

    // Query Notecard for temperature
    J * req = _notecard.newRequest("card.temp");
    J * rsp = _notecard.requestAndResponse(req);

    // Parse response
    if (JHasObjectItem(rsp,"value")) {
        J * temp_obj = JGetObjectItem(rsp,"value");
        if (JIsNumber(temp_obj)) {
            result = static_cast<float>(JNumberValue(temp_obj));
        }
    }

    // Clean up and return
    _notecard.deleteResponse(rsp);
    return result;
}
