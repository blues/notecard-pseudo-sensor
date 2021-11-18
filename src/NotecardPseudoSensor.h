#ifndef NOTECARD_PSEUDO_SENSOR_H
#define NOTECARD_PSEUDO_SENSOR_H

class Notecard;

namespace blues {

/*!
 * \brief Sensor class used for Notecard testing and examples
 *
 * NotecardPseudoSensor provides an API interface to the internal sensors of the
 * Blues Wireless Notecard. The goal of this abstraction is to offer a sensor to
 * use with more advanced tutorials, which enables focus on the syntax necessary
 * to perform basic Notecard transactions for those new to the platform.
 */
class NotecardPseudoSensor {
  public:
    /*!
     * \brief NotecardPseudoSensor (constructor)
     * \param[in] Notecard object
     * \warning The Notecard object provided MUST be intialized.
     */
    NotecardPseudoSensor(Notecard & notecard);

    /*!
     * \brief Generate humidity in %RH
     * \returns Percentage of relative humidity
     */
    float humidity (void);

    /*!
     * \brief Acquire temperature in centigrade
     * \returns The current temperature
     */
    float temp (void);

  private:
    Notecard & _notecard;
};

} // namespace

#endif // NOTECARD_PSEUDO_SENSOR_H
