#pragma once

// Is there anything more annoying? Honestly.
#define _CRT_SECURE_NO_WARNINGS 1

#include <assert.h>

#include <list>
#include <map>
#include <stdio.h>
#include <tuple>
#include <vector>

#include <d3d11.h>

// Needs to be included before SDL.h
#include "mathlib.h"

#include "SDL.h"
#include "dds_helper.h"



#ifdef _WIN32
#   pragma warning ( disable : 4351 )
#endif

