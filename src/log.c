#include "log.h"

void LOG_ERROR(char *fmt, ...)
{
    va_list argp;
    fprintf(stderr, BOLDRED "ERR: " RESET);
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    fprintf(stderr, "\n");
}

void LOG_WARN(char *fmt, ...)
{
    va_list argp;
    fprintf(stdout, BOLDYELLOW "WARN: " RESET);
    va_start(argp, fmt);
    vfprintf(stdout, fmt, argp);
    va_end(argp);
    fprintf(stdout, "\n");
}

void LOG_INFO(char *fmt, ...)
{
    va_list argp;
    fprintf(stdout, BOLDCYAN "LOG: " RESET);
    va_start(argp, fmt);
    vfprintf(stdout, fmt, argp);
    va_end(argp);
    fprintf(stdout, "\n");
}