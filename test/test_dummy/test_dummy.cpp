#include <gtest/gtest.h>
#include "j1939.h"

#if defined(ARDUINO)
#include <Arduino.h>

TEST(HeaderPGNTest, ShouldPass)
{
  J1939Header header = parseHeader(0x0CF00400);
  ASSERT_EQ(header.pgn, 61444);
}

TEST(TransmissionTest, ShouldPass)
{
  
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