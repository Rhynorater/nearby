// Copyright 2021 Google LLC.
#include "nearby_platform_bt.h"
#include "nearby_platform_os.h"
#include "nearby_platform_trace.h"

static const nearby_platform_BtCallbacks* bt_callbacks = NULL;

nearby_platform_status nearby_platform_BtInit(
    const nearby_platform_BtCallbacks* bt_interface) {
  NEARBY_TRACE_ENTRY();
  bt_callbacks = bt_interface;
  // TODO: Implement platform-specific BT initialization
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BtStartDiscovery() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT discovery start
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BtStopDiscovery() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT discovery stop
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BtConnect(uint64_t peer_address) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT connection
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BtDisconnect(uint64_t peer_address) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT disconnection
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BtSend(uint64_t peer_address,
                                            const uint8_t* data,
                                            size_t length) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT data sending
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BtGetRssi(uint64_t peer_address,
                                               int8_t* rssi) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT RSSI retrieval
  if (rssi) {
    *rssi = 0;
  }
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

bool nearby_platform_BtIsEnabled() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT enabled state check
  bool enabled = false;
  NEARBY_TRACE_RETURN(enabled);
  return enabled;
}

nearby_platform_status nearby_platform_BtSetEnabled(bool enabled) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT enabled state setting
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BtGetName(char* name, size_t length) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT name retrieval
  if (name && length > 0) {
    name[0] = '\0';
  }
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BtSetName(const char* name) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT name setting
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_BtGetAddress(uint64_t* address) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific BT address retrieval
  if (address) {
    *address = 0;
  }
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
} 