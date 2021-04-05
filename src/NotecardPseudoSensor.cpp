#include "NotecardPseudoSensor.h"

#ifndef MOCK
  #include <Notecard.h>
#else
  #include "Notecard.api.v1"
#endif

using namespace blues;

NotecardPseudoSensor::NotecardPseudoSensor (
    Notecard & notecard_
) :
    _notecard(notecard_)
{

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
