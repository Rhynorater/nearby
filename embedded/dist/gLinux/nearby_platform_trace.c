// Copyright 2021 Google LLC.
#include "nearby_platform_trace.h"
#include <stdio.h>

void nearby_platform_TraceEntry(const char* func) {
  printf("[TRACE ENTRY] %s\n", func);
}

void nearby_platform_TraceReturn(const char* func) {
  printf("[TRACE RETURN] %s\n", func);
}

void nearby_platform_TraceReturnValue(const char* func, int value) {
  printf("[TRACE RETURN] %s: %d\n", func, value);
}

void nearby_platform_TraceReturnPointer(const char* func, const void* ptr) {
  printf("[TRACE RETURN] %s: %p\n", func, ptr);
}

void nearby_platform_TraceReturnVoid(const char* func) {
  printf("[TRACE RETURN] %s: void\n", func);
} 