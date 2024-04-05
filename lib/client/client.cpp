#include "client.h"

void setupClient()
{
    delay(1000);

    client.begin("ssid", "password");

    int statusCode = client.get("/api/ets2/telemetry");
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
}