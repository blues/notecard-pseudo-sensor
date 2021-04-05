# Container with dependencies necessary to run unit-tests and code coverage

# Build development environment
# $ docker build . --tag notecard-pseudo-sensor-test

# Launch development environment
# $ docker run --rm --volume $(pwd)/..:/notecard-pseudo-sensor/ notecard-pseudo-sensor-test

# Base Image
FROM alpine

# Yarn Install Requirements
RUN ["ash", "-c", "\
    apk add --no-cache \
      gcovr \
      g++ \
      valgrind \
"]

WORKDIR /notecard-pseudo-sensor/test/
CMD ["./run_all_tests.sh"]
