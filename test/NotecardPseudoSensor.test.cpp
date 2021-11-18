#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch.hpp>
#include <fakeit.hpp>

// Compile Command: g++ ../src/NotecardPseudoSensor.cpp NotecardPseudoSensor.test.cpp mock/cjson.mock.cpp mock/stdlib.mock.cpp -Wall -Wextra -Wpedantic -I . -I ../src/ -I Catch2/single_include/ -I FakeIt/single_header/catch/ -DMOCK && ./a.out || echo "Tests Result: $?"

#include "NotecardPseudoSensor.h"
#include "mock/Notecard.api.v1"
#include "mock/stdlib.mock.hpp"

TEST_CASE("NotecardPseudoSensor::humidity() returns a base value of 45.0f", "[humidity]") {
    // Arrange
    rand_Parameters.reset();
    rand_Parameters.result = 0;
    fakeit::Mock<Notecard> mock_notecard;
    blues::NotecardPseudoSensor ps(mock_notecard.get());
    mock_notecard.ClearInvocationHistory();

    // Act
    const float result = ps.humidity();

    // Assert
    CHECK(result == 45.0f);
}

TEST_CASE("NotecardPseudoSensor::humidity() returns a max value of 49.9999f", "[humidity]") {
    // Arrange
    rand_Parameters.reset();
    rand_Parameters.result = RAND_MAX;
    fakeit::Mock<Notecard> mock_notecard;
    blues::NotecardPseudoSensor ps(mock_notecard.get());
    mock_notecard.ClearInvocationHistory();

    // Act
    const float result = ps.humidity();

    // Assert
    CHECK(result < 50.0f);
}

TEST_CASE("NotecardPseudoSensor::humidity() returns a value rounded to 4 decimal places", "[humidity]") {
    // Arrange
    fakeit::Mock<Notecard> mock_notecard;
    blues::NotecardPseudoSensor ps(mock_notecard.get());
    mock_notecard.ClearInvocationHistory();

    // Act
    const float result = ps.humidity();

    // Assert
    /*
     * Remove whole and decimal values to 4 places from
     * zero, then compare the resulting value to zero.
     */
    CHECK(0.0f == (static_cast<float>(result * 10000) - static_cast<int>(result * 10000)));
}

// plus a random number modulo 50000 divided by 10000", "[humidity]") {
TEST_CASE("NotecardPseudoSensor::temp() generates a `card.temp` request", "[temp]") {
    // Arrange
    JGetObjectItemResult = reinterpret_cast<J *>(0x19790917);
    JHasObjectItemResult = true;
    JIsNumberResult = true;
    JNumberValueResult = 9.17f;
    fakeit::Mock<Notecard> mock_notecard;
    fakeit::Fake(Method(mock_notecard,newRequest));
    fakeit::Fake(Method(mock_notecard,requestAndResponse));
    fakeit::Fake(Method(mock_notecard,deleteResponse));
    blues::NotecardPseudoSensor ps(mock_notecard.get());
    mock_notecard.ClearInvocationHistory();

    // Act
    ps.temp();

    // Assert
    //TODO: Bug in FakeIt v2.0.7 does not allow string literal comparisons
    // https://github.com/eranpeer/FakeIt/issues/232
    //fakeit::Verify(Method(mock_notecard,newRequest).Using("card.temp")).Once();
    fakeit::Verify(Method(mock_notecard,newRequest).Matching([](const char* str){return strcmp(str, "card.temp") == 0;})).Once();
    CHECK(true);
}

TEST_CASE("NotecardPseudoSensor::temp() submits the `card.temp` request to the Notecard", "[temp]") {
    // Arrange
    J * MOCK_OBJECT = reinterpret_cast<J *>(0x20130829);
    JGetObjectItemResult = reinterpret_cast<J *>(0x19790917);
    JHasObjectItemResult = true;
    JIsNumberResult = true;
    JNumberValueResult = 9.17f;
    fakeit::Mock<Notecard> mock_notecard;
    fakeit::When(Method(mock_notecard,newRequest)).AlwaysReturn(MOCK_OBJECT);
    fakeit::Fake(Method(mock_notecard,requestAndResponse));
    fakeit::Fake(Method(mock_notecard,deleteResponse));
    blues::NotecardPseudoSensor ps(mock_notecard.get());
    mock_notecard.ClearInvocationHistory();

    // Act
    ps.temp();

    // Assert
    fakeit::Verify(Method(mock_notecard,requestAndResponse).Using(MOCK_OBJECT)).Once();
    CHECK(true);
}

TEST_CASE("NotecardPseudoSensor::temp() returns the value parsed from the Notecard response", "[temp]") {
    // Arrange
    JGetObjectItemResult = reinterpret_cast<J *>(0x19790917);
    JHasObjectItemResult = true;
    JIsNumberResult = true;
    JNumberValueResult = 9.17f;
    fakeit::Mock<Notecard> mock_notecard;
    fakeit::Fake(Method(mock_notecard,newRequest));
    fakeit::Fake(Method(mock_notecard,requestAndResponse));
    fakeit::Fake(Method(mock_notecard,deleteResponse));
    blues::NotecardPseudoSensor ps(mock_notecard.get());
    mock_notecard.ClearInvocationHistory();

    // Act
    const float result = ps.temp();

    // Assert
    CHECK(result == JNumberValueResult);
}

TEST_CASE("NotecardPseudoSensor::temp() returns zero when no `value` key exists in the Notecard response", "[temp]") {
    // Arrange
    JGetObjectItemResult = reinterpret_cast<J *>(0x19790917);
    JHasObjectItemResult = false;
    JIsNumberResult = true;
    JNumberValueResult = 9.17f;
    fakeit::Mock<Notecard> mock_notecard;
    fakeit::Fake(Method(mock_notecard,newRequest));
    fakeit::Fake(Method(mock_notecard,requestAndResponse));
    fakeit::Fake(Method(mock_notecard,deleteResponse));
    blues::NotecardPseudoSensor ps(mock_notecard.get());
    mock_notecard.ClearInvocationHistory();

    // Act
    const float result = ps.temp();

    // Assert
    CHECK(result == 0.0f);
}

TEST_CASE("NotecardPseudoSensor::temp() returns zero when `value` key in the Notecard response is not a number", "[temp]") {
    // Arrange
    JGetObjectItemResult = reinterpret_cast<J *>(0x19790917);
    JHasObjectItemResult = true;
    JIsNumberResult = false;
    JNumberValueResult = 9.17f;
    fakeit::Mock<Notecard> mock_notecard;
    fakeit::Fake(Method(mock_notecard,newRequest));
    fakeit::Fake(Method(mock_notecard,requestAndResponse));
    fakeit::Fake(Method(mock_notecard,deleteResponse));
    blues::NotecardPseudoSensor ps(mock_notecard.get());
    mock_notecard.ClearInvocationHistory();

    // Act
    const float result = ps.temp();

    // Assert
    CHECK(result == 0.0f);
}

TEST_CASE("NotecardPseudoSensor::temp() deletes the Notecard response before exit", "[temp]") {
    // Arrange
    J * MOCK_OBJECT = reinterpret_cast<J *>(0x20180731);
    JGetObjectItemResult = reinterpret_cast<J *>(0x19790917);
    JHasObjectItemResult = true;
    JIsNumberResult = false;
    JNumberValueResult = 9.17f;
    fakeit::Mock<Notecard> mock_notecard;
    fakeit::Fake(Method(mock_notecard,newRequest));
    fakeit::When(Method(mock_notecard,requestAndResponse)).AlwaysReturn(MOCK_OBJECT);
    fakeit::Fake(Method(mock_notecard,deleteResponse));
    blues::NotecardPseudoSensor ps(mock_notecard.get());
    mock_notecard.ClearInvocationHistory();

    // Act
    ps.temp();

    // Assert
    fakeit::Verify(Method(mock_notecard,deleteResponse).Using(MOCK_OBJECT)).Once();
    fakeit::Verify(Method(mock_notecard,requestAndResponse) + Method(mock_notecard,deleteResponse));
    CHECK(true);
}
