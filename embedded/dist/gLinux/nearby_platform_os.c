// Copyright 2021 Google LLC.
#include "nearby_platform_os.h"
#include "nearby_platform_trace.h"

void* nearby_platform_Malloc(size_t size) {
  NEARBY_TRACE_ENTRY();
  void* ptr = malloc(size);
  NEARBY_TRACE_RETURN(ptr);
  return ptr;
}

void nearby_platform_Free(void* ptr) {
  NEARBY_TRACE_ENTRY();
  free(ptr);
  NEARBY_TRACE_RETURN_VOID();
}

void nearby_platform_SleepMs(uint32_t ms) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific sleep
  NEARBY_TRACE_RETURN_VOID();
}

uint64_t nearby_platform_GetCurrentTimeMs() {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific time retrieval
  uint64_t time = 0;
  NEARBY_TRACE_RETURN(time);
  return time;
}

void nearby_platform_RunOnMainThread(void (*callback)(void*), void* context) {
  NEARBY_TRACE_ENTRY();
  // TODO: Implement platform-specific main thread execution
  if (callback) callback(context);
  NEARBY_TRACE_RETURN_VOID();
} 