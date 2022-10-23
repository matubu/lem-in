#pragma once

#include <stdint.h>

/************/
/* Unsigned */
/************/
typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

#define	u8_MAX		UINT8_MAX
#define	u16_MAX		UINT16_MAX
#define	u32_MAX		UINT32_MAX
#define	u64_MAX		UINT64_MAX

#define	u8_MIN		UINT8_MIN
#define	u16_MIN		UINT16_MIN
#define	u32_MIN		UINT32_MIN
#define	u64_MIN		UINT64_MIN

/**********/
/* Signed */
/**********/
typedef int8_t		i8;
typedef int16_t		i16;
typedef int32_t		i32;
typedef int64_t		i64;

#define	i8_MAX		INT8_MAX
#define	i16_MAX		INT16_MAX
#define	i32_MAX		INT32_MAX
#define	i64_MAX		INT64_MAX

#define	i8_MIN		INT8_MIN
#define	i16_MIN		INT16_MIN
#define	i32_MIN		INT32_MIN
#define	i64_MIN		INT64_MIN

/********/
/* Bool */
/********/
typedef u8			bool;
#define true		1
#define false		0