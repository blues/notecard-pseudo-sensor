/*
 * Sleepy Sensor Application
 *
 * The goal of this application is to show the elements required to construct an
 * application which leverages the capabilities of the Notecard to minimize
 * battery consumption by controlling the wake time of the host MCU.
 *
 * Use the following diagram to wire up the circuit. Note the Notecard's `ATTN`
 * pin (exposed via the Notecarrier) is wired to the enable pin of the host MCU.
 *  ________________________________________
 * |  ____________________________________  |               ____________________
 * | |                                    | |              /
 * | |        _________________           | |             |  O
 * | |       |O|             |O|          | |             |     (Notecarrier-AL)
 * | --> SDA-|*  ___________  *|-21       | |       VUSB>-|[]
 * ----> SCL-|* | ESPRESSIF | *|-TX       | |  ----> BAT>-|[]
 *        14-|* |ESP32-WROOM| *|-RX       | |  |    MAIN>-|[]
 *        32-|* |CE         | *|-MI       | |  |     VIO>-|[]
 *        15-|* |           | *|-MO       | |  |          |[]
 *        33-|* |       ___ | *|-SCK      | |  |       V+-|[]
 *        27-|* |      |   || *|-A5    ---^-^--^----> GND-|[]
 *        12-|* |______|___|| *|-A4    |  | |  |       EN-|[]
 *        13-|*               *|-A3    |  | |  |      RST-|[]
 *       USB-|*               *|-A2    |  | |  |          |[]
 * -----> EN-|*         ----- *|-A1    |  | ---^----> SCL-|[]
 * | --> BAT-|*___      |   | *|-A0    |  -----^----> SDA-|[]
 * | |       |    |     ----- *|-GND <--       | --> ATTN-|[]
 * | |       |    |        _  *|-NC            | |  AUXEN-|[]
 * | |       |-----       |O| *|-3V            | |  AUXRX-|[]
 * | |       |      -----     *|-RST           | |  AUXTX-|[]
 * | |       |O___0_|___|_0___O|               | |   AUX1-|[]
 * | |                                         | |   AUX2-|[]
 * | |_________________________________________| |   AUX3-|[]
 * |_____________________________________________|   AUX4-|[]
 *                                                     RX-|[]
 *                                                     TX-|[]
 *                                                        |
 *                                                        |  O
 *                                                         \____________________
 *
 * NOTE: This sample is intended to compile for any Arduino compatible MCU
 * architecture and Notecard/Notecarrier combination. However, it was created
 * and tested using the Adafruit Huzzah32 and Notecarrier-AL.
 *
 * NOTE: This example has intentionally omitted error checking in order to
 * highlight the elements required to make a power-efficient application.
 */

#include <Notecard.h>
#include <NotecardPseudoSensor.h>
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#define productUID "com.your-company.your-name:sensor"
#define serialDebug Serial

// This period controls the waking frequency of your host MCU, and will have a
// direct impact on battery conservation. It should be used to strike a balance
// between battery performance and data collection requirements.
static const size_t PERIOD_S = 180;

Notecard notecard;
blues::NotecardPseudoSensor pseudo_sensor(notecard);

void setup() {
#ifdef ARDUINO_ARCH_ESP32
  // Disable radios to improve power profile
  WiFi.mode(WIFI_OFF);
  ::btStop();
#endif

  // Provide visual signal when the Host MCU is powered
  ::pinMode(LED_BUILTIN, OUTPUT);
  ::digitalWrite(LED_BUILTIN, HIGH);

  // Initialize Debug Output
  serialDebug.begin(115200);
  static const size_t MAX_SERIAL_WAIT_MS = 5000;
  size_t begin_serial_wait_ms = ::millis();
  while (!serialDebug && (MAX_SERIAL_WAIT_MS > (::millis() - begin_serial_wait_ms))) {
    ; // wait for serial port to connect. Needed for native USB
  }
  notecard.setDebugOutputStream(serialDebug);

  // Initialize Notecard
  notecard.begin();

  // Configure Notecard to synchronize with Notehub periodically, as well as
  // adjust the frequency based on the battery level
  {
    J * req = notecard.newRequest("hub.set");
    JAddStringToObject(req, "product", productUID);
    JAddStringToObject(req, "mode", "periodic");
    JAddStringToObject(req, "vinbound", "usb:60;high:120;normal:240;low:480;dead:0");
    JAddStringToObject(req, "voutbound", "usb:30;high:60;normal:90;low:120;dead:0");
    notecard.sendRequest(req);
  }

  // Optimize voltage variable behaviors for LiPo battery
  {
    J * req = notecard.newRequest("card.voltage");
    JAddStringToObject(req, "mode", "lipo");
    notecard.sendRequest(req);
  }

  // Establish a template to optimize queue size and data usage
  {
    J * req = notecard.newRequest("note.template");
    JAddStringToObject(req, "file", "sensor.qo");
    J * body = JAddObjectToObject(req, "body");
    JAddNumberToObject(body, "temp", 12.1);
    notecard.sendRequest(req);
  }

  // Sample Pseudo Sensor and queue results
  {
    float temperature = pseudo_sensor.temp();
    J * req = notecard.newRequest("note.add");
    JAddStringToObject(req, "file", "sensor.qo");
    J * body = JAddObjectToObject(req, "body");
    JAddNumberToObject(body, "temp", temperature);
    notecard.sendRequest(req);
  }
}

void loop() {
  // Request sleep from loop to safeguard against tranmission failure, and
  // ensure sleep request is honored so power usage is minimized.
  {
    // Create a "command" instead of a "request", because the host
    // MCU is going to power down and cannot receive a response.
    J * req = NoteNewCommand("card.attn");
    JAddStringToObject(req, "mode", "sleep");
    JAddNumberToObject(req, "seconds", PERIOD_S);
    notecard.sendRequest(req);
  }

  // Wait 1s before retrying
  ::delay(1000);
}
