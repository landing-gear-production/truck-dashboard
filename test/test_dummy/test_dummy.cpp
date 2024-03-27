#include <gtest/gtest.h>
#include "j1939.h"
#include "spoofer.h"

#if defined(ARDUINO)
#include <Arduino.h>

TEST(HeaderPGNTest, ShouldPass)
{
  auto response = parseHeader(0x0CF00400);
  ASSERT_EQ(response.pgn, 61444);
}

TEST(EngineSpeedMessageTest, ShouldPass)
{
  float rpm = 125;
  twai_message_t expected;
  expected.identifier = 0x0CF00400;
  expected.data_length_code = 8;
  expected.extd = 1;
  expected.data[4] = 0x03;
  expected.data[3] = 0xE8;

  auto response = createRpm(rpm);
  ASSERT_EQ(response.data[4], expected.data[4]);
  ASSERT_EQ(response.data[3], expected.data[3]);
}

TEST(ZeroEngineSpeed, ShouldPass)
{
  float rpm = 0;
  twai_message_t expected;
  expected.identifier = 0x0CF00400;
  expected.data_length_code = 8;
  expected.extd = 1;
  expected.data[4] = 0;
  expected.data[3] = 0;

  auto response = createRpm(rpm);
  ASSERT_EQ(response.data[4], expected.data[4]);
  ASSERT_EQ(response.data[3], expected.data[3]);
}

TEST(NavigationSpeed, ShouldPass)
{
  float rpm = 125;
  twai_message_t expected;
  expected.identifier = 0x18FEE8FE;
  expected.data_length_code = 8;
  expected.extd = 1;
  expected.data[3] = 0x03;
  expected.data[2] = 0xe8;

  auto response = createTacSpeed(rpm);
  ASSERT_EQ(response.data[3], expected.data[3]);
  ASSERT_EQ(response.data[2], expected.data[2]);
}

void setup()
{
  Serial.begin(115200);
  ::testing::InitGoogleTest();
}

void loop()
{
  if (RUN_ALL_TESTS());

  delay(1000);
}

#else
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS())
    ;

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
#endif