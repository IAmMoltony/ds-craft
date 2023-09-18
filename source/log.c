#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static LogLevel _logLevel = LOG_INFO;
static char *_logFileName = NULL;

static const char *_logLevelNames[] = {
    "INFO",
    "WARNING",
    "ERROR",
};

static char *_getTimeString(void)
{
    char *rnTime = malloc(28 * sizeof(char));
    struct tm *tminfo;
    time_t tmr = time(NULL);
    tminfo = localtime(&tmr);
    strftime(rnTime, 28, "%Y-%m-%d %H:%M:%S", tminfo);
    return rnTime;
}

void logInit(const LogLevel level, const char *logFileName)
{
    logSetLevel(level);
    _logFileName = (char *)logFileName;

    FILE *f = fopen(_logFileName, "w");
    if (!f)
    {
        perror(_logFileName);
        return;
    }

    char *rnTime = _getTimeString();
    fprintf(f, " *** Log Started %s\n", rnTime);
    free(rnTime);

    fclose(f);
}

void logSetLevel(const LogLevel level)
{
    _logLevel = level;
}

void logMessage(const LogLevel level, const char *format, ...)
{
    va_list l;
    va_list l2;
    va_start(l, format);
    va_copy(l2, l);

    if (level >= _logLevel)
    {
        printf("[%s] ", _logLevelNames[level]);
        vprintf(format, l);
        putchar('\n');
    }

    FILE *f = fopen(_logFileName, "a");
    if (!f)
    {
        perror(_logFileName);
        va_end(l);
        va_end(l2);
        return;
    }

	char *rnTime = _getTimeString();
    fprintf(f, "[%s] %s ", _logLevelNames[level], rnTime);
    free(rnTime);
    vfprintf(f, format, l2);
    fputc('\n', f);

    fclose(f);

    va_end(l);
    va_end(l2);
}
