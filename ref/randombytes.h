#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H

#include <stddef.h>
#include <stdint.h>

#include "fips202.h"

void randombytes(uint8_t *out, size_t outlen);

#endif
