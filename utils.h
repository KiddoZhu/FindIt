#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <typeinfo>

#pragma once

#define DEBUG
#ifdef DEBUG
	#define debug(...) printf(__VA_ARGS__)
#else
	#define debug
#endif