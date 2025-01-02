// Copyright (c) CedricZ1, 2024
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#include "Common.h"
#include "CommonHeaders.h"

#ifdef WIN32_MEAN_AND_LEAN
#define WIN32_MEAN_AND_LEAN
#endif // WIN32_MEAN_AND_LEAN


#include <Windows.h>

using namespace zone;

namespace {
HMODULE game_code_dll{ nullptr };
}	// anonymous namespace

EDITOR_INTERFACE uint32 LoadGameCodeDll(const char* dll_path)
{
	if (game_code_dll) return FALSE;

	game_code_dll = LoadLibraryA(dll_path);
	assert(game_code_dll);

	return game_code_dll ? TRUE : FALSE;
}

EDITOR_INTERFACE uint32 UnloadGameCodeDll()
{
	if (!game_code_dll) return FALSE;
	assert(game_code_dll);
	int result{ FreeLibrary(game_code_dll) };
	assert(result);
	game_code_dll = nullptr;

	return TRUE;
}