#pragma once

#ifdef _MSC_VER

#pragma warning(push, 4)

#pragma setlocale("russian")

#ifndef NDEBUG
#pragma check_stack(on)
#pragma strict_gs_check(push, on)
#endif // NDEBUG

#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1

#endif // _MSC_VER

#ifndef _WINDOWS_

typedef bool   BOOL;
typedef int    INT;
typedef size_t SIZE_T;
typedef float  FLOAT;
typedef double DOUBLE;
typedef void   VOID;
typedef char   CHAR;

#define CONST const

#define TRUE  true
#define FALSE false

#endif

