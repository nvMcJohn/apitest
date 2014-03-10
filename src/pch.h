#pragma once

// Is there anything more annoying? Honestly.
#define _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>

#include <algorithm>
#include <list>
#include <map>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <tuple>
#include <vector>

#ifdef _WIN32
#   define NOMINMAX 1
#   include <Windows.h>
#   include <d3d11.h>
#endif

// Needs to be included before SDL.h
#include "framework/console.h"
#include "framework/mathlib.h"

#include "SDL.h"
#include "framework/dds_helper.h"
#include "GL/glextensions.h"

#ifdef _WIN32
#   pragma warning ( disable : 4351 )
#endif

template <typename T>
void SafeRelease(T*& _p) { if (_p) { _p->Release(); _p = nullptr; } }

template <typename T> 
void SafeDelete(T*& _p) { delete _p; _p = nullptr; }

#define ArraySize(_arr) ( sizeof((_arr)) / sizeof((_arr)[0]) )
