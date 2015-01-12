#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "vm/vm_stdinc.h"

void log_init();
void log_end();
void log_write(const char *tag, const char *format, ...);

#define LOG_INFO(...)      (log_write("INFO", __VA_ARGS__))
#define LOG_WARN(...)      (log_write("WARN", __VA_ARGS__))
#define LOG_ERROR(...)     (log_write("ERROR", __VA_ARGS__))
#ifdef DEBUG
	#define LOG_DEBUG(...) (log_write("DEBUG", __VA_ARGS__))
#else
	#define LOG_DEBUG(...)
#endif

#ifdef __cplusplus
}
#endif
