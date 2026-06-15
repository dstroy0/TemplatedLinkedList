// Mock Arduino.h for native g++ compilation of unit tests.
// The library headers include <Arduino.h> but use no Arduino-specific
// functions or types in their template implementations.
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
