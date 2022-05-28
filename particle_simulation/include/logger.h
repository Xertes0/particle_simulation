#ifndef PS_LOGGER_H
#define PS_LOGGER_H

#include <stdio.h>

#define _COLOR_NRM "\x1B[0m"
#define _COLOR_RED "\x1B[31m"
#define _COLOR_GRN "\x1B[32m"
#define _COLOR_YEL "\x1B[33m"

#define ERR(...) \
	fprintf(stderr, _COLOR_RED"ERRO "_COLOR_NRM __VA_ARGS__ "\n");

#define WARN(...) \
	fprintf(stderr, _COLOR_YEL"WARN "_COLOR_NRM __VA_ARGS__ "\n");

#define INFO(...) \
	printf(_COLOR_GRN"INFO "_COLOR_NRM __VA_ARGS__ "\n");

#endif // PS_LOGGER_H
