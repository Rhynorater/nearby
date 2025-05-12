// Copyright 2021 Google LLC.
#include "nearby_platform_ble.h"
#include "nearby_platform_os.h"
#include "nearby_platform_trace.h"

static const nearby_platform_BleCallbacks* ble_callbacks = NULL;

nearby_platform_status nearby_platform_BleInit(
    const nearby_platform_BleCallbacks* ble_interface) {
  NEARBY_TRACE_ENTRY();
  ble_callbacks = ble_interface;
  // TODO: Implement platform-specific BLE initialization
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BleStartAdvertising(
    const nearby_platform_BleAdvertisementData* advertisement_data) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE advertising start
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BleStopAdvertising() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE advertising stop
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BleStartScanning(
    const nearby_platform_BleScanParameters* scan_parameters) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE scanning start
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BleStopScanning() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE scanning stop
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BleConnect(uint64_t peer_address) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE connection
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BleDisconnect(uint64_t peer_address) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE disconnection
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BleSend(uint64_t peer_address,
                                             const uint8_t* data,
                                             size_t length) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE data sending
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BleGetRssi(uint64_t peer_address,
                                                int8_t* rssi) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE RSSI retrieval
  if (rssi) {
    *rssi = 0;
  }
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

bool nearby_platform_BleIsEnabled() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE enabled state check
  bool enabled = false;
  NEARBY_TRACE_RETURN(enabled);
  return enabled;
}

nearby_platform_status nearby_platform_BleSetEnabled(bool enabled) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BLE enabled state setting
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
} 