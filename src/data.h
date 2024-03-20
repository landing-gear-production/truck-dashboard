#pragma once
#include <stdint.h>

enum SteeringWheelAngleSensorType {
  Relative = 0,
  Absolute = 1,
  Reserved = 2,
  NotAvailable = 3
};

struct VehicleState {
  uint16_t wheelBasedSpeed;
  float engineSpeed;
  uint8_t brakePedalPosition;
  uint8_t acceleratorPedalPosition;
  int8_t currentGear;
  SteeringWheelAngleSensorType steeringWheelAngleSensorType;
  float steeringWheelAngle;

  bool operator==(const VehicleState &other) const {
    return brakePedalPosition == other.brakePedalPosition &&
           acceleratorPedalPosition == other.acceleratorPedalPosition &&
           currentGear == other.currentGear &&
           steeringWheelAngleSensorType == other.steeringWheelAngleSensorType &&
           steeringWheelAngle == other.steeringWheelAngle &&
           wheelBasedSpeed == other.wheelBasedSpeed &&
           engineSpeed == other.engineSpeed;
  }

  bool operator!=(const VehicleState &other) const {
    return !(*this == other);
  }
};