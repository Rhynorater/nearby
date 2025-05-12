// Copyright 2021 Google LLC.
#include "nearby_platform_se.h"
#include "nearby_platform_trace.h"

static const nearby_platform_SeCallbacks* se_callbacks = NULL;

nearby_platform_status nearby_platform_SeInit(
    const nearby_platform_SeCallbacks* se_interface) {
  NEARBY_TRACE_ENTRY();
  se_callbacks = se_interface;
  // TODO: Implement platform-specific SE initialization
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_SeOpenSession() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific SE session open
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_SeCloseSession() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific SE session close
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_SeTransmit(const uint8_t* apdu, size_t apdu_len, uint8_t* response, size_t* response_len) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific SE transmit
  if (response_len) *response_len = 0;
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
} 