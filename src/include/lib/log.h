#ifndef LOG_ERROR_H
#define LOG_ERROR_H

#include <stdio.h>
#include <stdlib.h>

#define LOG_ERROR(fmt, ...) { fprintf(stderr, "[ERROR] %s:%i:%s: ", __FILE__, __LINE__, __func__); fprintf(stdout, fmt, ##__VA_ARGS__); fprintf(stdout, "\n"); exit(1); }
#define LOG_INFO(fmt, ...) { fprintf(stderr, "[INFO] %s:%i:%s: ", __FILE__, __LINE__, __func__); fprintf(stdout, fmt, ##__VA_ARGS__); fprintf(stdout, "\n"); }
#endif
