#!/bin/sh

g++ -fprofile-arcs -ftest-coverage -Wall -Wextra -Wpedantic -O0 -g \
  ../src/NotecardPseudoSensor.cpp \
  NotecardPseudoSensor.test.cpp \
  mock/cjson.mock.cpp \
  mock/stdlib.mock.cpp \
-I . \
-I ../src/ \
-I Catch2/single_include/ \
-I FakeIt/single_header/catch/ \
-DMOCK \
-DRUN_ALL_TESTS \
-o pseudo_sensor_tests \
&& valgrind ./pseudo_sensor_tests \
&& gcovr --print-summary --sort-percentage --exclude-throw-branches --delete \
  --object-directory . \
  --root ../src \
  --exclude .*_error.* \
&& rm pseudo_sensor_tests *.gcno
