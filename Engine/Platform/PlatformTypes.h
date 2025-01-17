// Copyright (c) CedricZ1, 2025
// Distributed under the MIT license. See the LICENSE file in the project root for more information.
#pragma once
#include "CommonHeaders.h"

#ifdef _WIN64
#ifndef WIN32_MEAN_AND_LEAN
#define WIN32_MEAN_AND_LEAN
#endif
#include <Windows.h>
namespace zone::platform
{

using window_proc = LRESULT(*)(HWND, UINT, WPARAM, LPARAM);
using window_handle = HWND;

struct WindowInitInfo 
{
	window_proc			callback{ nullptr };
	window_handle		parent{ nullptr };
	const wchar_t*		caption{ nullptr };
	int32				left{ 0 };
	int32				top{ 0 };
	int32				width{ 1920 };
	int32				height{ 1080 };
};

}

#endif // _WIN64