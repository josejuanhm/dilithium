///////////////////////////////////////////////////////////////////////////////////////
// October 19, 2023 - Modified by José Juan Hernández-Morales
//   Using RISC-V friendly randombytes() function, also defined in test_vectors.c
//   Repository: https://github.com/josejuanhm/fpau
///////////////////////////////////////////////////////////////////////////////////////

#ifndef RANDOMBYTES_H
#define RANDOMBYTES_H

#include <stddef.h>
#include <stdint.h>

#include "fips202.h"

void randombytes(uint8_t *out, size_t outlen);

#endif
