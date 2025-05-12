// Copyright 2021 Google LLC.
#include "nearby_platform_persistence.h"
#include "nearby_platform_trace.h"

nearby_platform_status nearby_platform_PersistenceInit() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific persistence initialization
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_PersistenceRead(const char* key, void* buffer, size_t* length) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific persistence read
  if (length) *length = 0;
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_PersistenceWrite(const char* key, const void* data, size_t length) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific persistence write
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
}

nearby_platform_status nearby_platform_PersistenceDelete(const char* key) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific persistence delete
  nearby_platform_status status = kNearbyStatusOK;
  NEARBY_TRACE_RETURN(status);
  return status;
} 