// Copyright (c) CedricZ1, 2024
// Distributed under the MIT license. See the LICENSE file in the project root for more information.

#include "Common.h"
#include "CommonHeaders.h"
#include "..\Engine\Components\Script.h"
#include "..\Graphics\Renderer.h"
#include "..\Platform\PlatformTypes.h"
#include "..\Platform\Platform.h"

#ifdef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN


#include <Windows.h>
#include <atlsafe.h>

using namespace zone;

namespace {
HMODULE game_code_dll{ nullptr };

using _get_script_creator = zone::script::detail::script_creator(*)(size_t);
_get_script_creator get_script_creator{ nullptr };

using _get_script_names = LPSAFEARRAY(*)(void);
_get_script_names get_script_names{ nullptr };

utl::vector<graphics::RenderSurface> surfaces;

}	// anonymous namespace

EDITOR_INTERFACE uint32 LoadGameCodeDll(const char* dll_path)
{
	if (game_code_dll) return FALSE;

	game_code_dll = LoadLibraryA(dll_path);
	assert(game_code_dll);

	get_script_creator = (_get_script_creator)GetProcAddress(game_code_dll, "get_script_creator");
	get_script_names = (_get_script_names)GetProcAddress(game_code_dll, "get_script_names");

	return (game_code_dll && get_script_creator && get_script_names) ? TRUE : FALSE;
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

EDITOR_INTERFACE script::detail::script_creator GetScriptCreator(const char* name)
{
	return (game_code_dll && get_script_creator) ? get_script_creator(script::detail::string_hash()(name)) : nullptr;
}

EDITOR_INTERFACE LPSAFEARRAY GetScriptNames()
{
	return (game_code_dll && get_script_names) ? get_script_names() : nullptr;
}

EDITOR_INTERFACE uint32 CreateRenderSurface(HWND host, int32 width, int32 height)
{
	assert(host);
	platform::WindowInitInfo info{ nullptr, host, nullptr, 0, 0, width, height };
	graphics::RenderSurface surface{ platform::createWindow(&info),{} };
	assert(surface.window.isValid());
	surfaces.emplace_back(surface);
	return (uint32)surfaces.size() - 1;
}

EDITOR_INTERFACE void ResizeRenderSurface(uint32 id)
{
	assert(id < surfaces.size());
	surfaces[id].window.resize(0, 0);
}


EDITOR_INTERFACE void RemoveRenderSurface(uint32 id)
{
	assert(id < surfaces.size());
	platform::removeWindow(surfaces[id].window.getID());
}

EDITOR_INTERFACE HWND GetWindowHandle(uint32 id)
{
	assert(id < surfaces.size());
	return (HWND)surfaces[id].window.handle();
}