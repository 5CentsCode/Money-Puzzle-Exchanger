#pragma once
#include <stdint.h>

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define NEW new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define  NEW new
#endif

#define GLM_ENABLE_EXPERIMENTAL

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;