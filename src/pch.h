#pragma once

// Is there anything more annoying? Honestly.
#define _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>

#include <list>
#include <map>
#include <stdio.h>
#include <string>
#include <tuple>
#include <vector>

#ifdef _WIN32
#   include <Windows.h>
#   include <d3d11.h>
#endif

// Needs to be included before SDL.h
#include "mathlib.h"

#include "SDL.h"
#include "dds_helper.h"

#ifdef _WIN32
#   pragma warning ( disable : 4351 )
#endif

#define ArraySize(_arr) ( sizeof((_arr)) / sizeof((_arr)[0]) )