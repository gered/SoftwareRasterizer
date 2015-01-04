#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define SWAP(T, a, b) \
	do {              \
		T tmp = a;    \
		a = b;        \
		b = tmp;      \
	} while (0)

