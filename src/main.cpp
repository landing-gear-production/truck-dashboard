#include <main.h>

VehicleState state;

void setup() {
  Serial.begin(115200);
  CAN0.setCANPins(CAN_RX_PIN, CAN_TX_PIN);
  CAN0.watchFor();

  if (CAN0.begin(500000))
    Serial.println("CAN0 init ok");
  else
    Serial.println("CAN0 init failed");
}

void loop() {
  VehicleState oldState = state;
  CAN_FRAME frame;

  if (CAN0.read(frame)) {
    onData(&frame);

    if (oldState != state) {
      Serial.printf("Brake: %d, Accelerator: %d, Steering: %f, Current Gear: %d\n", state.brakePedalPosition, state.acceleratorPedalPosition, state.steeringWheelAngle, state.currentGear);
    }
  }
}

void onData(CAN_FRAME *frame) {
  // all J1939 frames are extended
  if (!frame->extended)
    return;

  J1939Header header = parseHeader(frame->id);
  uint8_t gear, type, rawAngle;
  float radians;

  switch (header.pgn) {
    case 61441:
      // brake pedal position
      state.brakePedalPosition = frame->data.bytes[1];
      break;
    case 61443:
      // accelerator pedal position
      state.acceleratorPedalPosition = frame->data.bytes[1];
      break;
    case 61445:
      // extract transmission information
      gear = frame->data.bytes[0];
      state.currentGear = gear - 125;
      break;
    case 61449:
      // extract steering wheel angle sensor type
      type = frame->data.bytes[2];
      state.steeringWheelAngleSensorType = (SteeringWheelAngleSensorType) getBits(type, 0, 2);

      if (state.steeringWheelAngleSensorType == SteeringWheelAngleSensorType::NotAvailable)
        return;
  
      // todo: check if LE or BE for raw angle
      rawAngle = frame->data.bytes[1] << 8 | frame->data.bytes[0];
      radians = ((float)rawAngle) * 31.374;
      state.steeringWheelAngle = radians * 180 / PI;
      break;
  }
}

