#include "vm_log.h"

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <sys/timeb.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#endif

FILE *fp_log = NULL;
const char *log_filename = "run.log";

const size_t LOG_BUFFER_SIZE = 8096;
char _temp_buffer[LOG_BUFFER_SIZE];
char _buffer[LOG_BUFFER_SIZE];

void log_init() {
	fp_log = fopen(log_filename, "w");
	if (!fp_log)
	{
		snprintf(_buffer, LOG_BUFFER_SIZE, "Log file \"%s\" could not be opened. Further LOG_Write() calls will only appear in STDOUT.\n", log_filename);
		printf("%s", _buffer);
#ifdef _MSC_VER
		OutputDebugStringA(_buffer);
#endif
	}
}

void log_end() {
	if (fp_log)
		fclose(fp_log);
}

void log_write(const char *tag, const char *format, ...) {
	static char timestamp[15];
	
	int hour;
	int minute;
	int second;
	unsigned int milliseconds;
	
	struct timeb seconds;
	ftime(&seconds);
	struct tm *currentDate = localtime(&seconds.time);
	
	hour = currentDate->tm_hour;
	minute = currentDate->tm_min;
	second = currentDate->tm_sec;
	milliseconds = seconds.millitm;
	
	// Prepend the date/time
	snprintf(timestamp, 15, "[%02d:%02d:%02d,%03d]", hour, minute, second, milliseconds);
	
	// now the user-provided format string and arguments...
	va_list args;
	va_start(args, format);
	vsnprintf(_temp_buffer, LOG_BUFFER_SIZE, format, args);
	va_end(args);
	
	// write it all out
	snprintf(_buffer, LOG_BUFFER_SIZE, "%s [%s] %s", timestamp, tag, _temp_buffer);
	
	if (fp_log)
		fprintf(fp_log, "%s", _buffer);
	printf("%s", _buffer);
#ifdef _MSC_VER
	OutputDebugStringA(_buffer);
#endif
}
